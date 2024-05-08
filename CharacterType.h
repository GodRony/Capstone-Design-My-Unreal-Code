#pragma once
UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied,
	EAS_HitReaction,
	EAS_Attacking,
	EAS_Dead
};
enum class EAIPossessState : uint8
{
	EPS_AIPossess,
	EPS_PlayerPossess,
};
UENUM(BlueprintType)
enum EDeathPose 
{
	EDP_Death1,
	EDP_Death2,
	EDP_Death3,
	EDP_Death4,
	EDP_Death5,
	EDP_MAX
};
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState,
	EES_Dead,
	EES_Patrolling,
	EES_Chasing,
	EES_Attacking,
	EES_Engaged,

};
UENUM(BlueprintType)
enum class EAttackState : uint8
{
	EATS_UniqueAbilityState,
	EATS_SwordState,
	EATS_NoState
};
UENUM(BlueprintType)
enum class ESkillAttribute : uint8
{
	ESS_Pysical,
	ESS_Fire,
	ESS_Ice,
	ESS_Electric,
	ESS_Gale

};
