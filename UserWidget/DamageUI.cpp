// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/DamageUI.h"
#include "Components/TextBlock.h"
void UDamageUI::ShowDamageUI(float Damage)
{
    
  
    DamageText->SetText(FText::AsNumber(Damage));
    //DamageWidget->AddToViewport();
  
    
}
