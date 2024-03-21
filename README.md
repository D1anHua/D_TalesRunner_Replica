### 项目说明
本项目原本是超级跑跑的复刻版本(未完成品), 由于找暑期实习临近, 故而更新一下自己的项目进展.

本项目构思是做一款多人联机竞技竞速网游. 欲参考**UE5 Lyra Sample Game**示例项目, 由于目前该项目并未完结, 现展示对**Lyra 的框架的学习与解读**以及部分项目构思及展现.

项目配置:
- UE5.2 (Epic Version)

~~本项目是超级跑跑(英文名:`TalesRunner`)的`UE`复刻版本(抄袭版).~~

~~2024.3.13更新: 说来挺惭愧的, 本来认为能够在找实习之前做出来大致的样子的, 但现在感觉不行了. 只能将目前所完成的部分设计构思以及架构学习笔记更新一下了.~~

~~*版权声明: 本项目仅仅是`Demo`性质的, 不包含任何盈利部分, 存在的目的仅仅是用作检验`UE`的学习成果.* **如有侵犯原游戏的任何利益, 可联系我立即删除.**~~

以下解读文档也可参考个人博客: [TalesRunner超级跑跑UE复刻版(抄袭版)构建过程](https://www.d1anhua.top/2024/03/21/TalesRunner%E8%B6%85%E7%BA%A7%E8%B7%91%E8%B7%91UE%E5%A4%8D%E5%88%BB%E7%89%88-%E6%8A%84%E8%A2%AD%E7%89%88-%E6%9E%84%E5%BB%BA%E8%BF%87%E7%A8%8B/)

## LyraExperience模块解读.

`Lyra`添加了新的`Experience`模块, 其官方包括网上的一些资料都介绍说它是`GameMode`的高级版本. 今天, 将首先从`LyraExperience`这个模块来讲解这个项目.

**注:**个人也是UE新手, 如果文章中有错误, 希望大家可以批评指出, 我会尽量改正, 避免给大家造成误解.

(PS: 纯看这个项目的话, 确实感觉可以从`LyraExperience`这个模块开始看, 但是我个人感觉这个项目在各个模块之间的耦合度、以及对`Tag && Assets Data`的使用超乎我的想象. 要是想要重新复刻这个项目的话, 我建议从`Character && Modular Gameplay`看起)

> 先说一些碎碎念吧:
> 我本身并不是`UE`老手, 最近想做一个`DEMO`项目好让自己在求职季多点姿色. 当时想通过学习`Lyra`项目来学习一些`Epic`官方的代码风格, 以及学习一下他们是如何构建项目的.
>
> 结果不出所料地是, 我完全低估了`Lyra`项目的难度, 结果不可避免的让自己的`Demo`没有办法按期完成了(🤨), 让自己当初的想法显得天真了一些.
>
> **不过必须要说的是**: 虽然自己的`Demo`是要错过暑期实习的时间了, 不过秋招还是可以努努力的. 另外就是, `lyra`这个项目还是可以让人学到很多东西的.
>
> 剩下的就不多念叨了. 提前感谢大家的观看了.

我的参考链接放在开头, 因为我感觉我自己的文章本身出自别人, 并且自己写的质量也不一定有别人高. 感兴趣的可以直接看别人的文章呀.\
1. [知乎---UE5 新项目Gameplay框架设计（以Lyra为例）](https://zhuanlan.zhihu.com/p/614718286)\
2. [知乎---UE5 Lyra的数据驱动之数据加载与引用](https://zhuanlan.zhihu.com/p/505766201)\
3. [知乎---《InsideUE5》GameFeatures架构（一）发展由来](https://zhuanlan.zhihu.com/p/467236675)\
4. [Youtube---Modular Game Features in UE5: plug ‘n play, the Unreal way](https://www.youtube.com/watch?v=3PBnqC7TxvM)\
5. [官方文档: Game Featrues and Modular Gameplay](https://docs.unrealengine.com/5.1/en-US/game-features-and-modular-gameplay-in-unreal-engine/)\

### What is Experience
我个人的理解的话, 故名思意, 就可以理解为是一种游玩的体验, 不用的`Experience`可能就是不同的游戏风格, 其中包含有角色相关的、控制映射相关的、也有和功能(GameFeature)相关的. 至于它和以前的`Gamemode`的区别以及优势. 我个人其实对`Gamemode`的理解也并不深刻. 所以我也就不在这里大放厥词了. 具体的功能以及方便与否还是由个人自己区分的(或者有大佬愿意讲讲哈哈哈)

**注:** 在介绍之前, 方便的同学可以先看看参考链接4或者5以简单的了解一下`GameFeatrue`. `GameFeatrue`在`Lyra`中也是深度集成的.\

不了解也行, 就知道它是一种在**不影响游戏原有Component运行过程**之外, 所添加的一些额外的功能.\
这里我简单举例就像加入我现在只有一个能行走的角色. 我可以将其他任何的逻辑都通过`GameFeatrue`在不影响游戏本身以及各个部件之间运行的前提下去增加诸如UI、新的功能等等, 可以丰富这个游戏.(大钊老师的举例很形象. 往主板上拓展的思路)

#### LyraExperienceDefinition
`ExperienceDefinition`定义如下:
![](https://hexoblog-1304281944.cos.ap-hongkong.myqcloud.com/hexo/pic/2024-03-13-22-09-01.png)

> 在UE5中展示如下:
> ![](https://hexoblog-1304281944.cos.ap-hongkong.myqcloud.com/hexo/pic/2024-03-13-21-54-32.png)
> 其设置位置在`World Setting-->GameMode`下:
> ![](https://hexoblog-1304281944.cos.ap-hongkong.myqcloud.com/hexo/pic/2024-03-13-22-00-12.png)


> 注: 这里设置的位置其实是`Lyra`继承了`AWorldSettings`(可以在以后的工作中也尝试尝试):
> ``` cpp
> class LYRAGAME_API ALyraWorldSettings : public AWorldSettings{
>  	UPROPERTY(EditDefaultsOnly, Category=GameMode)
>   TSoftClassPtr<ULyraExperienceDefinition> DefaultGameplayExperience; 
> ... }
> ```

`Experience`总共由四个成员变量组成, 其中除了第一个之外全是Lyra的自定义类型. 这里我简单介绍一下, 有个概念就行.(这四部分就是整个`Experience`所提供的服务的全部了, 可以说):\
1. GameFeaturesToEnable: 将要使用的是哪一系列的`GameFeatures`.
2.


## LyraModularGameplay && ModularGameplayActor && Enhanced Input(Tag)模块解读

`Lyra`项目在整个`Character`的构建过程中, 都运用了`ModularGameplay`这个插件. 刚入手一个项目的第一步, 我个人觉得还是应该先看看`Lyra`角色是怎么设置的.

本文章主要包含`Lyra`的以下几个部分:\
1. ModularGameplay\
2. Enhanced Input Mapping(同`Tag`绑定)\
3. LyraPawnExtensionComp && LyraHeroComp 之间的Initialization State System.\
   本部分不会涉及`GAS`系统相关的部分, `GAS`会放后面讲.

**注:**我本人也是UE学习的新手, 对基础概念掌握并不深, 如果有分析错误的地方, 还请大家批评指正, 也避免误导大家.

参考链接如下:\
1. [官方文档: Game Framework Component Manager](https://docs.unrealengine.com/5.1/en-US/game-framework-component-manager-in-unreal-engine/)

### ModularGameplayActor
`Lyra`插件目录中有一个插件:`ModularGameplayActor`, 其中包含了常用的Actor的`MudoularGameplay`的实现, 为在[参考链接1](https://docs.unrealengine.com/5.1/en-US/game-framework-component-manager-in-unreal-engine/)中的第一部分`Extension Handler System`提供了默认的实现.
![](https://hexoblog-1304281944.cos.ap-hongkong.myqcloud.com/hexo/pic/2024-03-18-16-35-24.png)\

在每一个`ModularGameplayActor`的实现中, 最主要的在`PreInitalizeComponents()`中注册成为了Reveiver, 并在`EndPlay()`中`RemoveReceiver`.\

> ``` cpp
> // ModularCharacter.cpp
> void AModularCharacter::PreInitializeComponents()
> {
> 	Super::PreInitializeComponents();
> 	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
> }
> void AModularCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
> {
> 	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
> 	Super::EndPlay(EndPlayReason);
> }
> void AModularCharacter::BeginPlay()
> {
>   //! 注册了时间名为NAME_GameActorReady的Default Event(这个事件声明在GameFrameworkComponentManager.h的头文件中), 方便其他插件使用. 比如做同步控制时
> 	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
> 	Super::BeginPlay();
> }
> 
> // ModularGameMode.cpp
> AModularGameMode::AModularGameMode(const FObjectInitializer& ObjectInitializer)
> 	: Super(ObjectInitializer)
> {
> 	GameStateClass = AModularGameState::StaticClass();
> 	PlayerControllerClass = AModularPlayerController::StaticClass();
> 	PlayerStateClass = AModularPlayerState::StaticClass();
> 	DefaultPawnClass = AModularPawn::StaticClass();
> }
> ```

以后可以直接继承这个插件来实现自己`GameFeaturePlugins`的实现.

### Input System
**Lyra**的`Input System`可与`GAS`、`GameplayTag`以及不同的控制器协调工作, 其在`EnhancedInput`的基础上进行了封装.

#### Input System Data Stucture
![](https://hexoblog-1304281944.cos.ap-hongkong.myqcloud.com/hexo/pic/2024-03-18-19-16-36.png)

下面放一张正常的使用`Enhanced Input System`的代码截图, 从代码中可以看出来, 关于`Enhanced Input System`主要有两部分:

- InputMappingContext's Bind\
- InputActor's Bind\

因此呢, 对应的`Lyra`分别定义了这两部分的数据结构.
![](https://hexoblog-1304281944.cos.ap-hongkong.myqcloud.com/hexo/pic/2024-03-18-17-21-02.png)

首先是**InputMappingContext**, `Lyra`中直接使用了`UPlayerMappableInputConfig`, 其中维护了一个含有`InputMappingContext`的Map.

Lyra系统为这个`Config`封装了`CommonInputType`(*这个枚举类型用来说明所使用的控制器的类型*)信息. 并通过HeroComponent来设置.
![](https://hexoblog-1304281944.cos.ap-hongkong.myqcloud.com/hexo/pic/2024-03-18-18-12-22.png)

而另一方面**InputActions**则对齐封装了`GameplayTag`信息形成了`LyraInputAction`. 并根据其时候是`Ability`而分为了`Native`和`Ability`之分. 并通过`PawnData`进行设置.
(**注:** 这里的`PawnData`并不是简单的在`PawnExtensionComp`中进行设置的, 在`Lyra`其是在`Experience`的初始化过程中由`GameState`完成的)
![](https://hexoblog-1304281944.cos.ap-hongkong.myqcloud.com/hexo/pic/2024-03-18-18-16-10.png)


