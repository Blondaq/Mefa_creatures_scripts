class mefa_Gondor_Anorien_archers_dummy : mefa_Humanoid_Logic_Base {

	protected float         m_Scale = 1.1;
	void mefa_Gondor_Anorien_archers_dummy()
	{
		SetEventMask(EntityEvent.POSTFRAME);
		RegisterNetSyncVariableFloat("m_Scale");
		SetSimpleSelectionVisible("arrow", false);

	}

	/**
	* Register additional custom animation events
	*/
	override void RegisterAnimEvents()
	{
		super.RegisterAnimEvents();

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			RegisterAnimationEvent("ScriptHideArrow", "OnScriptClientEvent");
			RegisterAnimationEvent("ScriptShowArrow", "OnScriptClientEvent");
		}

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			//RegisterAnimationEvent("ScriptHideArrow", "OnScriptServerEvent");
			//RegisterAnimationEvent("ScriptShowArrow", "OnScriptServerEvent");

		}
	}

	/**
	* Process client side custom scripted events
	*/
	void OnScriptClientEvent(int event_id, string event_user_string)
	{
		switch (event_id)
		{
			case 1:
			{
				SetSimpleSelectionVisible("arrow", false);
				//GetGame().ChatPlayer("Hiding Arrow - Client");
				break;
			}
			case 2:
			{
				SetSimpleSelectionVisible("arrow", true);
				//GetGame().ChatPlayer("Showing Arrow - Client");
				break;
			}
		}
	}

	/**
	 * Process server side custom scripted events
	 */
	void OnScriptServerEvent(int event_id, string event_user_string)
	{
		switch (event_id)
		{
		case 1:
		{
			//GetGame().ChatPlayer("Hiding Arrow - Server");
			break;
		}
		case 2:
		{
			//GetGame().ChatPlayer("Showing Arrow - Server");
			break;
		}
		}
	}






	override void EEInit()
	{
		super.EEInit();


	}


	override void EOnPostFrame(IEntity other, int extra) // | EntityEvent.POSTFRAME
	{
		vector mat[4];
		GetTransform(mat);

		mat[0] = mat[0].Normalized() * m_Scale;
		mat[1] = mat[1].Normalized() * m_Scale;
		mat[2] = mat[2].Normalized() * m_Scale;

		SetTransform(mat);
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		ZombieBase zombie = ZombieBase.Cast(source);
		if (zombie)
		{
			float EnemyDamageType = GetEnemyDamageType(zombie);
			if (EnemyDamageType == 0) //DEFAULT
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "DEFAULT", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 1) //SLASH
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SLASH", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 2) //PIERCE
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "PIERCE", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 3) //SPECIALIST
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SPECIALIST", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 4) //CAVALRY
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "CAVALRY", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 5) //CRUSH
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "CRUSH", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 6) //SIEGE
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "SIEGE", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 7) //FLAME
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "FLAME", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 8) //FROST
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "FROST", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 9) //MAGIC
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "MAGIC", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 10) //HERO
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "HERO", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 11) //HERO_RANGED
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "HERO_RANGED", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 12) //STRUCTURAL
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "STRUCTURAL", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
			else if (EnemyDamageType == 13) //POISON
			{
				adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "POISON", armorModifier);

				ApplyDamage(dmgZone, adjustedDamage);
			}
		}
		return false;  // Vr�t� false, aby se zabr�nilo dal��mu zpracov�n� po�kozen�
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);



	}

	void FireProjectile(ZombieBase zombie, vector targetPosition) {
		vector rightHandPosition = this.ModelToWorld(this.GetMemoryPointPos("firingFromBowZed"));
		Object projectile;
		projectile = Object.Cast(GetGame().CreateObject("mefa_Anorien_Archers_Arrow", rightHandPosition));
		projectile.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
		//projectile.SetDynamicPhysicsLifeTime(5); // decay physics
		float newHeight;
		float newWidthX;

		if (projectile) {

			if (GetGame().IsServer()) {
				if (isTargetPlayer) {
					if (isPlayerProning) {
						//GetGame().ChatPlayer("proning");
						newHeight = targetPosition[1] - 0.9; // Lower target position if prone
						targetPosition[1] = newHeight;
					}
					else if (isPlayerCrouching) {
						//GetGame().ChatPlayer("crouching");
						newHeight = targetPosition[1] - 0.5; // Lower target position if crouching
						targetPosition[1] = newHeight;
					}
					else {
						// No adjustment needed if standing
						//GetGame().ChatPlayer("standing");
					}
				}
			}

			float hitChance = 1.0; // Nastavte to na hodnotu, kterou chcete pro �anci na z�sah (0.5 pro 50%)
			float missTypeChance = 0.5;
			float randomValue = Math.RandomFloatInclusive(0.0, 1.0); // Generuje n�hodn� ��slo mezi 0.0 a 1.0
			float randomValue2 = Math.RandomFloatInclusive(0.0, 1.0); // Generuje n�hodn� ��slo mezi 0.0 a 1.0
			vector direction;
			vector finalImpulse;
			float impulseStrength;
			float distance;
			float verticalBoost;
			vector angles;
			if (randomValue <= hitChance) { // Pokud je n�hodn� ��slo men�� nebo rovno �anci na z�sah
				// V�po�et a aplikace impulsu ��pu
				direction = targetPosition - rightHandPosition;
				direction.Normalize();

				distance = vector.Distance(rightHandPosition, targetPosition);
				impulseStrength = CalculateImpulseStrength(distance);
				verticalBoost = CalculateVerticalBoost(distance);

				finalImpulse = direction * impulseStrength + vector.Up * verticalBoost;
				angles = direction.VectorToAngles();
				projectile.SetOrientation(angles);

				dBodyApplyImpulse(projectile, finalImpulse);
			}
			else {

				if (randomValue2 <= missTypeChance) {

					newWidthX = targetPosition[0] + 2.0;
					targetPosition[0] = newWidthX;
				}
				else
				{
					newWidthX = targetPosition[0] - 2.0;
					targetPosition[0] = newWidthX;
				}



				direction = targetPosition - rightHandPosition;
				direction.Normalize();

				distance = vector.Distance(rightHandPosition, targetPosition);
				impulseStrength = CalculateImpulseStrength(distance);
				verticalBoost = CalculateVerticalBoost(distance);

				finalImpulse = direction * impulseStrength + vector.Up * verticalBoost;
				angles = direction.VectorToAngles();
				projectile.SetOrientation(angles);

				dBodyApplyImpulse(projectile, finalImpulse);
				//GetGame().ChatPlayer("Miss!");

			}
		}
	}

	float CalculateImpulseStrength(float distance) {
		if (distance <= 5.0) {
			return 155.0;  // Siln�j�� impuls pro velmi kr�tkou vzd�lenost
		}
		else if (distance <= 10.0) {
			return 145.0;  // Sn�en� impulzu, ale st�le siln� pro kr�tk� vzd�lenosti
		}
		else if (distance <= 15.0) {
			return 140.0;  // Udr�en� impulzu pro ide�ln� dopad na 15m
		}
		else if (distance <= 20.0) {
			return 135.0;  // Standardn� impuls, dobr� pro 20m
		}
		else if (distance <= 25.0) {
			return 130.0;  // M�rn� sn�en� pro v�t�� vzd�lenosti
		}
		else if (distance <= 30.0) {
			return 130.0 + 0.4 * (distance - 25.0);  // Postupn� zvy�uj�c� se impuls
		}
		else {
			return 132.0 + 0.8 * (distance - 30.0);  // Zna�n� zv��en� impulzu pro vzd�lenosti nad 30m
		}
	}

	float CalculateVerticalBoost(float distance) {
		if (distance <= 5.0) {
			return 52.0;  // Vy��� boost pro velmi kr�tkou vzd�lenost, aby se zas�hla vy��� oblast, ne kolena
		}
		else if (distance <= 10.0) {
			return 30.0;  // Zv��en� boostu, aby ��p �el trochu nad p�s
		}
		else if (distance <= 15.0) {
			return 26.0;  // Zv��en� boostu, c�l�me v��e ne� je pas
		}
		else if (distance <= 20.0) {
			return 28.0;  // Zv��en�, aby se zabr�nilo kles�n� ��pu tak rychle
		}
		else if (distance <= 25.0) {
			return 28.0 + 1.0 * (distance - 20.0);  // Postupn� zvy�ov�n� boostu pro del�� vzd�lenosti
		}
		else if (distance <= 30.0) {
			return 32.0 + 1.0 * (distance - 25.0);  // Dal�� zvy�ov�n� boostu
		}
		else {
			return 36.0 + 1.0 * (distance - 30.0);  // Velk� zv��en� boostu pro vzd�lenosti nad 30m
		}
	}




	bool isTargetPlayer = false;
	bool isPlayerStanding = false;
	bool isPlayerCrouching = false;
	bool isPlayerProning = false;
	override bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{


		m_ActualTarget = pInputController.GetTargetEntity();

		//! do not attack players in vehicle - hotfix
		PlayerBase pb = PlayerBase.Cast(m_ActualTarget);
		if (pb && pb.GetCommand_Vehicle())
			return false;


		if (m_ActualTarget == null || IsTargetAlive(m_ActualTarget) == false)
		{
			ResetTarget();
			return false;
		}

		if (pb)
		{
			isPlayerProning = false;
			isPlayerCrouching = false;
			isPlayerStanding = false;
			isTargetPlayer = true;
			if (pb.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
			{
				isPlayerProning = true;

			}
			else if (pb.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH))
			{
				isPlayerCrouching = true;
			}
			else
			{
				isPlayerStanding = true;
			}
		}

		vector targetPos = m_ActualTarget.GetPosition();
		float targetDist = vector.Distance(targetPos, this.GetPosition());
		int pitch = GetAttackPitch(m_ActualTarget);

		float target_distance = vector.DistanceSq(m_ActualTarget.GetPosition(), GetPosition());
		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.FIGHT, targetDist, pitch);
		{

			Object target = DayZPlayerUtils.GetMeleeTarget(this.GetPosition(), this.GetDirection(), 20, 11, -1.0, 2.0, this, m_TargetableObjects, m_AllTargetObjects);

			if (m_AllTargetObjects.Count() > 0 && m_AllTargetObjects[0] != m_ActualTarget)
			{
				m_AllTargetObjects.Clear();
				return false;
			}

			m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.CHASE, targetDist, pitch);
			if (targetDist < 35.0)
			{
				int randomAttackType = Math.RandomIntInclusive(0, 2); // nahodny utok 5 /6  jeste by mel zmrazit hraci kontrol ovladani

				//SetSimpleSelectionVisible("arrow", false);
				//int randomAttackType = 0;
				//GetGame().ChatPlayer("Vybran nahodny utok: " + randomAttackType);   //  0- Default utok //  5- right attack
				StartCommand_Attack(m_ActualTarget, randomAttackType, 0);

				m_AttackCooldownTime = 0.5;
				return true;
			}
			else
			{
				return false;
			}


		}
		return false;
	}
	override bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		// always update target - it can be destroyed		
		m_ActualTarget = pInputController.GetTargetEntity();
		
	
		
		//! do not attack players in vehicle - hotfix
		PlayerBase pb = PlayerBase.Cast(m_ActualTarget);
		if (pb && pb.GetCommand_Vehicle())
		{
			return false;
		}

		if (pb)
		{
			isPlayerProning = false;
			isPlayerCrouching = false;
			isPlayerStanding = false;
			isTargetPlayer = true;
			if (pb.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
			{
				isPlayerProning = true;

			}
			else if (pb.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH))
			{
				isPlayerCrouching = true;
			}
			else
			{
				isPlayerStanding = true;
			}
		}


		if (m_ActualTarget == null || IsTargetAlive(m_ActualTarget) == false)
		{
			ResetTarget();
			return false;
		}

		vector targetPos = m_ActualTarget.GetPosition();
		if (!CanAttackToPosition(targetPos))
			return false;

		float targetDist = vector.Distance(targetPos, this.GetPosition());
		int pitch = GetAttackPitch(m_ActualTarget);

		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.CHASE, targetDist, pitch);
		if (targetDist < 35.0)   // attack za behu
		{
			FireProjectile(this, targetPos);
			//GetGame().ChatPlayer("Utok za behu: ");
			StartCommand_Attack(m_ActualTarget, 5, 0);
			m_AttackCooldownTime = 0.2;
			return true;
		}
		else if (targetDist < 40.0)   // attack za behu
		{

			return true;
		}
		return false;
	}

	bool IsTargetAlive(EntityAI target)
	{
		if (target)
		{
			float targetHealth = target.GetHealth();
			//GetGame().ChatPlayer("health is: " + targetHealth.ToString());
			if (targetHealth <= 0)
			{
				//GetGame().ChatPlayer("target is dead");
				return false;
			}
			else
			{
				//GetGame().ChatPlayer("target is alive");
				return true;
			}
		}
		return false;
	}

	


	void ResetTarget()
	{
		//SetSimpleSelectionVisible("arrow", false);
		m_ActualTarget = null;
		//GetGame().ChatPlayer("Target was nullified");

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