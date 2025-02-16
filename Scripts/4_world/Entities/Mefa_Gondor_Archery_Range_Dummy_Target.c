class Mefa_Gondor_Archery_Range_Dummy_Target : mefa_Humanoid_Logic_Base {


	override void EEInit()
	{
		super.EEInit();


	}


	
	
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		ItemBase projectile = ItemBase.Cast(source);

		if (projectile)
		{
			if (projectile.IsKindOf("mefa_Anorien_Archers_Arrow")) {
				
				

			}
			GetGame().ObjectDelete(source);
			
		}
		
		return false;  // Vrátí false, aby se zabránilo dalšímu zpracování poškození
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