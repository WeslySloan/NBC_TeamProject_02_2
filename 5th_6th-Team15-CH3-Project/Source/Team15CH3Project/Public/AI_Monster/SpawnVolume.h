#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_Monster/MonsterSpawnRow.h"
#include "SpawnVolume.generated.h"

// 전방 선언
class USceneComponent;
class UBoxComponent;
class UDataTable;

UCLASS()
class TEAM15CH3PROJECT_API ASpawnVolume : public AActor
{
    GENERATED_BODY()

public:
    ASpawnVolume();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartWaves();

    UFUNCTION(BlueprintPure, Category = "Wave")
    int32 GetCurrentWave() const { return CurrentWave; }

    UFUNCTION(BlueprintPure, Category = "Wave")
    int32 GetPlayerLevel() const { return PlayerLevel; }

    // ===== Wave 시간 설정 =====
    // 1웨이브 기본 시간
    UPROPERTY(EditAnywhere, Category = "Wave|Time")
    float WaveDuration = 60.f;

    // 웨이브마다 감소(초) ★ 에러 메시지의 변수 이름
    UPROPERTY(EditAnywhere, Category = "Wave|Time")
    float WaveDurationDecrementPerWave = 1.f;

    // 웨이브 시간 하한 ★ 에러 메시지의 변수 이름
    UPROPERTY(EditAnywhere, Category = "Wave|Time")
    float MinWaveDuration = 10.f;

    // 전장 클리어 시 즉시 다음 웨이브
    UPROPERTY(EditAnywhere, Category = "Wave|Time")
    bool bSpawnNextWaveWhenCleared = true;

    // ===== 스폰 수 설정 =====
    UPROPERTY(EditAnywhere, Category = "Wave|Spawn")
    int32 BaseMonstersPerWave = 10;

    UPROPERTY(EditAnywhere, Category = "Wave|Spawn")
    int32 IncrementPerWave = 5;

    // ===== (선택) 전부 처치 레벨업 판정용 =====
    // ★ 에러 메시지의 변수 이름
    UPROPERTY(EditAnywhere, Category = "Wave|LevelUp")
    float KillAllTimeLimitWave1 = 50.f;

    UPROPERTY(EditAnywhere, Category = "Wave|LevelUp")
    float KillAllTimeReducePerWave = 5.f;

    UPROPERTY(EditAnywhere, Category = "Wave|LevelUp")
    float KillAllTimeMin = 20.f;

protected:
    // ===== 기존 스폰 구성 =====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* MonsterDataTable;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnRandomMonster();

    FVector GetRandomPointInVolume() const;
    FMonsterSpawnRow* GetRandomMonster() const;
    AActor* SpawnMonster(TSubclassOf<AActor> Monster);

    // ===== 웨이브 상태 =====
    UPROPERTY(VisibleAnywhere, Category = "Wave")
    int32 CurrentWave = 0;

    UPROPERTY(VisibleAnywhere, Category = "Wave")
    int32 PlayerLevel = 1;

    float WaveStartTimeSec = 0.f;
    int32 SpawnedThisWave = 0;
    int32 KilledThisWave = 0;

    FTimerHandle WaveTimerHandle;

    // ===== 웨이브 헬퍼 =====
    void StartNextWave();
    void SpawnForCurrentWave_UsingYourPattern();
    float ComputeWaveDuration(int32 WaveIndex) const;   // ★ 선언 필수
    void ArmNextWaveTimer();                            // ★ 선언 필수
    void OnAllKilledCheckAndMaybeLevelUp();

    UFUNCTION()
    void HandleSpawnedActorDestroyed(AActor* DestroyedActor);


};
