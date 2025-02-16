class Mefa_Gondor_Archery_Range_Damage_Receiver : mefa_Humanoid_Logic_Base {


	override void EEInit()
	{
		super.EEInit();


	}


	
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		float damage = damageResult.GetDamage(dmgZone, "Health");
		
		
		ZombieBase zombie = ZombieBase.Cast(source);
		ItemBase projectile = ItemBase.Cast(source);

		if (projectile)
		{
			if (projectile.IsKindOf("mefa_Anorien_Archers_Arrow")) {
				
				float EnemyDamageTypeItem = GetEnemyDamageTypItem(projectile);
				

				if (EnemyDamageTypeItem == 0) //DEFAULT
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "DEFAULT", armorModifier);
					GetGame().ChatPlayer("DEFAULT item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 1) //SLASH
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SLASH", armorModifier);
					GetGame().ChatPlayer("SLASH item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 2) //PIERCE
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "PIERCE", armorModifier);
					GetGame().ChatPlayer("PIERCE item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 3) //SPECIALIST
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SPECIALIST", armorModifier);
					GetGame().ChatPlayer("SPECIALIST item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 4) //CAVALRY
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "CAVALRY", armorModifier);
					GetGame().ChatPlayer("CAVALRY item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 5) //CRUSH
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "CRUSH", armorModifier);
					GetGame().ChatPlayer("CRUSH item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 6) //SIEGE
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SIEGE", armorModifier);
					GetGame().ChatPlayer("SIEGE item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 7) //FLAME
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "FLAME", armorModifier);
					GetGame().ChatPlayer("FLAME item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 8) //FROST
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "FROST", armorModifier);
					GetGame().ChatPlayer("FROST item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 9) //MAGIC
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "MAGIC", armorModifier);
					GetGame().ChatPlayer("MAGIC item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 10) //HERO
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "HERO", armorModifier);
					GetGame().ChatPlayer("HERO item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 11) //HERO_RANGED
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "HERO_RANGED", armorModifier);
					GetGame().ChatPlayer("HERO_RANGED item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 12) //STRUCTURAL
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "STRUCTURAL", armorModifier);
					GetGame().ChatPlayer("STRUCTURAL item adjustedDamage: " + adjustedDamage.ToString());
					TransferDamage(adjustedDamage);
				}
				else if (EnemyDamageTypeItem == 13) //POISON
				{
					adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "POISON", armorModifier);
					GetGame().ChatPlayer("POISON item adjustedDamage: " + adjustedDamage.ToString());

					TransferDamage(adjustedDamage);
				}

			}
			GetGame().ObjectDelete(source);
			
		}
		
		else if (zombie)
		{
			float EnemyDamageType = GetEnemyDamageType(zombie);
			if (EnemyDamageType == 0) //DEFAULT
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "DEFAULT", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 1) //SLASH
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SLASH", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 2) //PIERCE
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "PIERCE", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 3) //SPECIALIST
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SPECIALIST", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 4) //CAVALRY
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "CAVALRY", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 5) //CRUSH
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "CRUSH", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 6) //SIEGE
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SIEGE", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 7) //FLAME
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "FLAME", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 8) //FROST
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "FROST", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 9) //MAGIC
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "MAGIC", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 10) //HERO
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "HERO", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 11) //HERO_RANGED
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "HERO_RANGED", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 12) //STRUCTURAL
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "STRUCTURAL", armorModifier);

				TransferDamage(adjustedDamage);
			}
			else if (EnemyDamageType == 13) //POISON
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "POISON", armorModifier);

				TransferDamage(adjustedDamage);
			}
		}
		return false;  // Vr�t� false, aby se zabr�nilo dal��mu zpracov�n� po�kozen�
	}

	void TransferDamage(float damageToTransfer)
	{
		float radius = 15.0;
		vector checkPosition = this.GetPosition();
		array<Object> nearbyObjects = new array<Object>();
		array<CargoBase> proxyCargos = new array<CargoBase>();

		GetGame().GetObjectsAtPosition3D(checkPosition, radius, nearbyObjects, proxyCargos);
		for (int i = 0; i < nearbyObjects.Count(); i++)
		{
			Object obj = nearbyObjects.Get(i);

			if (obj.IsKindOf("Mefa_Building_Gondor_Archery_Range_lvl_1"))
			{
				float currentHealth;
				if (GetGame().IsServer())
				{
					currentHealth = obj.GetHealth();
				}
				obj.SetHealth("", "", (currentHealth - damageToTransfer));
			}
		}
		delete nearbyObjects;
		delete proxyCargos;

	}


	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		float damage = damageResult.GetDamage(dmgZone, "Health");
		//GetGame().ChatPlayer("source: " + source.GetType());
		//GetGame().ChatPlayer("damage: " + damage.ToString());
	}

	protected void SetSimpleSelectionVisible(string selection, bool visible)
	{
		array<string> selections = {};
		ConfigGetTextArray("simpleHiddenSelections", selections);

		int index = selections.Find(selection);
		if (index != -1)
		{
			SetSimpleHiddenSelectionState(index, visible);
		}
	}


}