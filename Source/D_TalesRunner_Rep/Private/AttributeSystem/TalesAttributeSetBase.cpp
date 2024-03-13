// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/TalesAttributeSetBase.h"

UWorld* UTalesAttributeSetBase::GetWorld() const
{
	// 自我感觉和原版没啥区别
	const UObject* Outer = GetOuter();
	check(Outer);
	return Outer->GetWorld();
}
