enum CreaturelCmd
{
	WILD,
	PLRCONTROL
}

class mefa_Humanoid_Logic_Base extends ZombieBase
{
	private bool m_IsControlled = false;  // Přidána proměnná určující, zda je ork kontrolován
	float armorModifier = 0;
	float adjustedDamage;
	protected PlayerBase m_ControllingPlayer;  // Reference na hráče, který kontroluje orka

	protected int m_CreaturelCmd;
	protected AIAgent m_AIAgent;

  	float MaxHealth;
    int CreatureLevel;
    float soulEnergy;
    float softCapHealth;

	 // Přidejte příznak, zda již byl spuštěn timer pro přidávání EXP
    protected bool m_ExpTimerStarted = false;


	void mefa_Humanoid_Logic_Base()
	{
		RegisterNetSyncVariableInt("m_CreaturelCmd");
		SetCreaturelCmd(CreaturelCmd.WILD);

		 // Při spawnu načíst HP a upravit podle controlu hráče
        InitHealth();
	}

	void InitHealth()
    {
        soulEnergy = GetGame().ConfigGetFloat("CfgVehicles " + this.GetType() + " SoulEnergy");
        softCapHealth = GetGame().ConfigGetFloat("CfgVehicles " + this.GetType() + " SoftCapHealth");
        float baseHealth = softCapHealth + soulEnergy;
        // Předpokládáme, že hráčův level je uložen v nějaké proměnné nebo získaný z Level Stone
        int currentLevel = CreatureLevel; // nebo jiný zdroj
        MaxHealth = GetMaxHealthForLevel(currentLevel, baseHealth);
        // Nastavíme HP na maximum (při prvotní inicializaci)
        SetHealth("", "Health", MaxHealth);
    }

	// Metoda, která bude periodicky volána (např. každou sekundu) pro přidání EXP
	void ExpTest()
	{
		// Spustíme pouze, pokud je kreatura živá
		if (IsCreatureAlive())
		{
			AddExperienceNoncontrolled(500);
		}
	}

		 // Metoda pro přidání EXP do EXP Stone pro non-controlled kreatury
    void AddExperienceNoncontrolled(float expAmount)
    {
        // Najdeme EXP Stone pro non-controlled kreatury v inventáři (slot "CreaturesExperiences")
        EntityAI expStone = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("CreaturesExperiences"));
        if (!expStone)
        {
           // GetGame().ChatPlayer("Non-controlled EXP Stone nebyl nalezen v inventáři!");
            return;
        }
        // Přičteme novou hodnotu EXP
        float currentExp = expStone.GetQuantity();
        float newExp = currentExp + expAmount;
        expStone.SetQuantity(newExp);
       // GetGame().ChatPlayer("Non-controlled: Přidáno " + expAmount.ToString() + " EXP. Nová EXP hodnota: " + newExp.ToString());
        
        // Po přidání EXP aktualizujeme úroveň kreatury
        UpdateExperienceNoncontrolled();
    }



    // Nová metoda pro aktualizaci max HP bez resetu aktuálního HP
    void UpdateMaxHealthFromLevel(int newLevel)
    {
        // Uložíme aktuální HP a vypočteme poměr
        float currentHP = GetHealth("", "Health");
       	float healthRatio;
		if (MaxHealth > 0)
		{
			healthRatio = currentHP / MaxHealth;
		}
		else
		{
			healthRatio = 1.0;
		}

	
        // Vypočteme nové max HP
        float baseHealth = softCapHealth + soulEnergy;
        float newMaxHP = GetMaxHealthForLevel(newLevel, baseHealth);

        // Aktualizujeme interní proměnnou
        MaxHealth = newMaxHP;

        // Nastavíme nové HP, zachovávající poměr (např. pokud bylo 70 % před, zůstane 70 %)
        float newCurrentHP = newMaxHP * healthRatio;
        SetHealth("", "Health", newCurrentHP);

        // Aktualizujeme level kreatury (pro případné další logiky)
        CreatureLevel = newLevel;
    }

    // Metoda pro výpočet max HP (můžeš upravit podle potřeb)
    float GetMaxHealthForLevel(int playerLevel, float baseHealth)
    {
        float growthFactor = 1.05; // 5 % růst za level
        float multiplier = Math.Pow(growthFactor, playerLevel - 1); // exponenciální růst
        return baseHealth * multiplier;
    }

    // Metoda, která byla volána při změně levelu (původně SetCreatureLevel)
    void SetCreatureLevel(int level)
    {
        // Pokud chceme resetovat HP při prvním připojení, můžeme volat InitHealth()
        // Ale pokud se mění level během boje, použijeme UpdateMaxHealthFromLevel, aby se damage zachovalo.
        UpdateMaxHealthFromLevel(level);
    }

	// V rámci třídy mefa_Humanoid_Logic_Base (která již dědí ze ZombieBase)
	// Metoda pro výpočet úrovně a aktualizaci Level Stone pro non-controlled kreatury
    void UpdateExperienceNoncontrolled()
    {
        // Najdeme EXP Stone a Level Stone pro non-controlled kreatury
        EntityAI expStone = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("CreaturesExperiences"));
        EntityAI lvlStone = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("CreaturesLevel"));
        if (expStone && lvlStone)
        {
            float expValue = expStone.GetQuantity();
            // Definujeme prahy pro každý level (stejné hodnoty jako u hráče – případně je upravte podle potřeb)
            float expThresholds[61] = { 0, 1000, 2000, 3099, 4308, 5637, 7099, 8706, 10473, 12415, 14551, 16900, 19482, 22322, 25443, 28876, 32650, 36800, 41362, 46379, 51895, 57960, 64628, 71960, 80022, 88886, 98632, 109348, 121130, 134085, 148329, 163990, 181210, 200144, 220962, 243851, 269018, 296690, 327116, 360569, 397352, 437795, 482263, 531155, 584914, 644022, 709012, 780470, 859039, 945427, 1040411, 1144848, 1259678, 1385935, 1524757, 1677393, 1845219, 2029747, 2232637, 2455718, 2700000 };
            
            int creatureLevel = 1; // Výchozí úroveň
            for (int i = 0; i < 60; i++)
            {
                if (expValue >= expThresholds[i] && expValue < expThresholds[i + 1])
                {
                    creatureLevel = i + 1;
                    break;
                }
            }
            if (expValue >= 2700000)
            {
                creatureLevel = 60;
            }
            // Nastavíme úroveň do Level Stone
            lvlStone.SetQuantity(creatureLevel);
            // Aktualizujeme úroveň kreatury (což přepočítá HP)
            SetCreatureLevel(creatureLevel);
        }
    }




	void SetControlled(bool state) {
        m_IsControlled = state;
    }

    bool IsControlled() {
        return m_IsControlled;
    }

	 void SetControllingPlayer(PlayerBase player) {
        m_ControllingPlayer = player;
    }

    PlayerBase GetControllingPlayer() {
        return m_ControllingPlayer;
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
        super.OnRPC(sender, rpc_type, ctx);
        // Pro případné další RPC implementace, pokud budete posílat zprávy mezi klientem a serverem
    }

    // Příklad metody odpojení:
    void ResetControl() {
        m_ControllingPlayer = null;  // Odpojení hráče
    }


	override void EOnFrame(IEntity other, float timeSlice)
	{
		if (!GetGame().IsServer()) // Zajištění, že kód běží pouze na serveru
			return;

		 // Pokud tato kreatura není ovládána hráčem, spustíme timer pro přidávání EXP
        if (!m_IsControlled && !m_ExpTimerStarted)
        {
            m_ExpTimerStarted = true;
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.ExpTest, 1000, true);
        }

		 // Aktualizovat zkušenostní systém pouze u non-controlled kreatur
		if (!m_IsControlled)
		{
			UpdateExperienceNoncontrolled();
		}

		 // Pokud nechceš posouvat hráče za creaturou, přeskoč to:
		/*if (!m_ControllingPlayer || !m_IsControlled)
			return;
		*/
		// Kontrola, zda je tato creatura označena, a výpis informací
		if (m_ControllingPlayer && this) // Kontrola, zda jsou reference platné
		{
			// Získání aktuální pozice orka
			vector creaturePosition = this.GetPosition();
			vector creatureOrientation = this.GetOrientation(); // Získání orientace orka

			// Výpočet posunuté pozice hráče (1 metr za orkem)
			float offsetHeight = 0.0; // Výška hráče nad terénem
			// Nahraď např. větší hodnotou:
			float distanceBehind = 0.8; // nebo 3.0, dle tvých potřeb

			// Převod orientace na směr (vektor)
			vector direction = Vector(Math.Sin(creatureOrientation[0] * Math.DEG2RAD), 0, Math.Cos(creatureOrientation[0] * Math.DEG2RAD));
			vector playerPosition = creaturePosition - (direction * distanceBehind); // Posun hráče o 1 metr za orka
			playerPosition[1] = creaturePosition[1] + offsetHeight; // Nastavení výšky hráče

			// Nastavení hráče na vypočtenou pozici
			m_ControllingPlayer.SetPosition(playerPosition);

			// Simulace "šlapání vody" nebo běhu ve vzduchu
			HumanCommandMove hcm = m_ControllingPlayer.GetCommand_Move();
			if (!hcm) // Pokud hráč právě neprovádí žádnou akci pohybu
			{
				m_ControllingPlayer.StartCommand_Move(); // Zahájení základního pohybu
			}

			// Zajištění, že hráč není ovlivněn fyzikou (např. gravitací nebo kolizemi)
			m_ControllingPlayer.PhysicsEnableGravity(false); // Vypnutí gravitace
			m_ControllingPlayer.COTSetRemoveCollision(true, true); // Deaktivace kolizí
		}


	}


	/*override void EOnFrame(IEntity other, float timeSlice) {
        super.EOnFrame(other, timeSlice);
		Input input = GetGame().GetInput(); // Získání vstupu od hráče
        if (m_IsControlled) 
		{
			if (input.LocalPress("UAFire")) // Například stisknutí levého tlačítka myši
			{
				GetGame().ChatPlayer("Attack");
			}		
        }
		
    }*/

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		ZombieBase zombie = ZombieBase.Cast(source);
		ItemBase projectile = ItemBase.Cast(source);
		float EnemyDamageType;
		float EnemyDamageTypeItem;
		float EnemyDamageTypeItem2;
		if (projectile) 
		{
			if (projectile.IsKindOf("mefa_Anorien_Archers_Arrow")) {
				{
					this.StartCommand_Hit(true, 0, -25.0);
					EnemyDamageTypeItem = GetEnemyDamageTypItem(projectile);
					if (EnemyDamageTypeItem == 2) //PIERCE
					{
						adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "PIERCE", armorModifier);
						//GetGame().ChatPlayer("PIERCE item adjustedDamage: " + adjustedDamage.ToString());
						ApplyDamage(dmgZone, adjustedDamage);
					}
					//return false;
				}
			}
			else if (projectile.IsKindOf("mefa_Anorien_Archers_Arrow_Fire_Upgrade")) {
				{
					this.StartCommand_Hit(true, 0, -25.0);
					EnemyDamageTypeItem = GetEnemyDamageTypItem(projectile);
					EnemyDamageTypeItem2 = GetEnemyDamageTypItem2(projectile);
					if (EnemyDamageTypeItem == 2) //PIERCE
					{
						adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "PIERCE", armorModifier);
						//GetGame().ChatPlayer("PIERCE item adjustedDamage: " + adjustedDamage.ToString());
						ApplyDamage(dmgZone, adjustedDamage);
					}
					if (EnemyDamageTypeItem2 == 7) //FLAME
					{
						adjustedDamage = CalculateAdjustedDamage(damageResult, dmgZone, "FLAME", armorModifier);
						//GetGame().ChatPlayer("PIERCE item adjustedDamage: " + adjustedDamage.ToString());
						ApplyDamage(dmgZone, adjustedDamage);
					}

					//return false;
				}
			}
		}
			

		else if (zombie)
		{
			EnemyDamageType = GetEnemyDamageType(zombie);
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
		

		return false;  // Vr t  false, aby se zabr nilo dal  mu zpracov n  po kozen 
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		float causedDamageValue;
	
		int objectSize;
		int boneIndex;
		vector localPosition;
		string objectClassName;
		Object objectInstance;
		vector arrowOrientation;
		if (source.GetType() == "mefa_Anorien_Archers_Arrow")
		{
			causedDamageValue = damageResult.GetDamage(dmgZone, "Health");
			// GetGame().ChatPlayer("dmgZone is: " + dmgZone);
		   //  GetGame().ChatPlayer("component is: " + component.ToString());
		   //  GetGame().ChatPlayer("modelPos is: " + modelPos.ToString());
		   //  GetGame().ChatPlayer("damage is: " + causedDamageValue.ToString());





			objectClassName = "mefa_Anorien_Archers_Arrow";
			localPosition = "0 0 0";
			objectSize = 1;
			boneIndex = this.GetBoneIndexByName(dmgZone);
			if (boneIndex != -1) {
				objectInstance = GetGame().CreateObject(objectClassName, this.GetPosition(), false, true, true);

				// Z sk n  orientace   pu
				arrowOrientation = source.GetOrientation();

				objectInstance.SetPosition(localPosition);
				objectInstance.SetOrientation(arrowOrientation);
				this.AddChild(objectInstance, boneIndex);
			}
			else {
				//GetGame().ChatPlayer("Invalid bone: " + dmgZone);
			}

			GetGame().ObjectDelete(source);
		}
		

	}

	override bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		// always update target - it can be destroyed		
		m_ActualTarget = pInputController.GetTargetEntity();
		
		//! do not attack players in vehicle - hotfix
		PlayerBase pb = PlayerBase.Cast(m_ActualTarget);
		if ( pb && pb.GetCommand_Vehicle() )
		{
			return false;
		}

		if ( m_ActualTarget == NULL )
			return false;
	
		vector targetPos = m_ActualTarget.GetPosition();
		if ( !CanAttackToPosition(targetPos) )
			return false;
		
		float targetDist = vector.Distance(targetPos, this.GetPosition());
		int pitch = GetAttackPitch(m_ActualTarget);
		
		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.CHASE, targetDist, pitch);
		if (m_ActualAttackType)
		{
			Object target = DayZPlayerUtils.GetMeleeTarget(this.GetPosition(), this.GetDirection(), TARGET_CONE_ANGLE_CHASE, m_ActualAttackType.m_Distance, -1.0, 2.0, this, m_TargetableObjects, m_AllTargetObjects);
			//! target is outside the targeting cone; skip attack
			if (m_ActualTarget != target)
			{
				m_AllTargetObjects.Clear();
				return false;
			}

			StartCommand_Attack(m_ActualTarget, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype);
			m_AttackCooldownTime = m_ActualAttackType.m_Cooldown;
			return true;
		}
		
		return false;
	}
	
	override bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		// always update target - it can be destroyed		
		m_ActualTarget = pInputController.GetTargetEntity();
		
		//! do not attack players in vehicle - hotfix
		PlayerBase pb = PlayerBase.Cast(m_ActualTarget);
		if (pb && pb.GetCommand_Vehicle())
			return false;

		if (m_AttackCooldownTime > 0)
		{
			m_AttackCooldownTime -= pDt * GameConstants.AI_ATTACKSPEED;
			return false;
		}
					
		if (m_ActualTarget == null)
			return false;

		vector targetPos = m_ActualTarget.GetPosition();
		float targetDist = vector.Distance(targetPos, this.GetPosition());
		int pitch = GetAttackPitch(m_ActualTarget);
		
		if (!CanAttackToPosition(targetPos))
			return false;

		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.FIGHT, targetDist, pitch);
		if (m_ActualAttackType)
		{
			Object target = DayZPlayerUtils.GetMeleeTarget(this.GetPosition(), this.GetDirection(), TARGET_CONE_ANGLE_FIGHT, m_ActualAttackType.m_Distance, -1.0, 2.0, this, m_TargetableObjects, m_AllTargetObjects);
			//! target is outside the targeting cone; skip attack
			if (m_AllTargetObjects.Count() > 0 && m_AllTargetObjects[0] != m_ActualTarget)
			{
				m_AllTargetObjects.Clear();
				return false;
			}

			StartCommand_Attack(m_ActualTarget, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype);
			m_AttackCooldownTime = m_ActualAttackType.m_Cooldown;
			return true;
		}

		return false;
	}

	bool IsCreatureAlive()
	{
		if(this.GetHealth() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}

	}


	bool IsTargetAlive(EntityAI target)
	{
		if (target)
		{
			return target.GetHealth() > 0;
		}
		return false;
	}


	void ResetTarget()
	{
		
		m_ActualTarget = null;
		
	}

	protected void RepositionFromTarget(EntityAI target, float desiredDistance)
	{
		if (!target) return;

		vector myPos = GetPosition();
		vector targetPos = target.GetPosition();

		// Vektor od cíle k nám
		vector dir = myPos - targetPos;

		// Pokud by byl 0 (stejná pozice), dej mu aspoň nějakou orientaci
		if (dir.Length() < 0.01)
		{
			dir = "0 0 1";
		}

		dir.Normalize();

		// Zkus lehce "rozhodit" úhel, aby nestála creatura vždy jen přímo v linii
		dir[0] = dir[0] + Math.RandomFloatInclusive(-0.2, 0.2);
		dir[2] = dir[2] + Math.RandomFloatInclusive(-0.2, 0.2);
		dir.Normalize();

		// Nová pozice s odstupem desiredDistance
		vector newPos = targetPos + (dir * desiredDistance);

		// Ujisti se, že stojíme na zemi
		newPos[1] = GetGame().SurfaceY(newPos[0], newPos[2]);

		// Přímo nastavíme pozici (jednoduché řešení)
		SetPosition(newPos);
	}


	protected float m_AutoAttackDisableTimer = 0.0;  // Časovač pro zpoždění autoattacku po pohybu
	protected const float AUTOATTACK_DISABLE_COOLDOWN = 2.0;  // Délka cooldownu po pohybu v sekundách	
	override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return true;
		}
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			return true;
		}
		
		if (this.GetType().IndexOf("Controlled") == -1)
		{
			//GetGame().ChatPlayer("Creature does not contain 'Controlled' in its class name.");
			return false;
		}
		
		DayZInfectedInputController inputController = this.GetInputController();

		if (m_IsControlled) {
			SetCreaturelCmd(CreaturelCmd.PLRCONTROL);
		}
		else
		{
			SetCreaturelCmd(CreaturelCmd.WILD);
		}

		m_AIAgent = this.GetAIAgent();
		m_AIAgent.SetKeepInIdle(true); 
		PlayerBase player = GetControllingPlayer();
		float plrspeed = 0.0;   // Rychlost hráče
		vector plrdirection;    // Směr pohybu

		if (player == null)
		{
			return false;  // Ukonči, pokud není hráč připojen
		}

		vector playerPos = player.GetPosition();
		vector creaturePos = this.GetPosition();
		float distance = vector.Distance(playerPos, creaturePos);

		HumanInputController hic = player.GetInputController();
		if (hic == null)
		{
			//GetGame().ChatPlayer("Error: HumanInputController is null.");
			return false;  // Ukonči, pokud neexistuje HumanInputController
		}

		// Snížení časovače cooldownu
		if (m_AutoAttackDisableTimer > 0.0)
		{
			m_AutoAttackDisableTimer -= pDt;
		}

		if (this.GetCreaturelCmd() == CreaturelCmd.PLRCONTROL)
		{
			if (this.GetCommand_Move() == null && plrdirection.Length() > 0.1)
			{
				this.StartCommand_Move();
			}

			if (plrdirection.Length() > 0.1)  // Kontrola pohybu hráče
			{
			// GetGame().ChatPlayer("Player is moving, autoattack interrupted.");
				ResetAutoAttack();  // Přeruší autoattack, pokud hráč běží
				m_AutoAttackDisableTimer = AUTOATTACK_DISABLE_COOLDOWN;  // Nastav cooldown
			}

			// Pokud cooldown ještě nevypršel, neprováděj autoattack
			if (m_AutoAttackDisableTimer > 0.0)
			{
			// GetGame().ChatPlayer("Autoattack on cooldown, waiting...");
				return true;
			}

			// Spusť autoattack logiku
			AutoAttack(pDt);

			// Pohyb během autoattacku
			HandleMovementDuringAutoAttack(pDt, hic);

			return true;
		}
		return false;

	}

	bool HandleMovementDuringAutoAttack(float pDt, HumanInputController hic)
	{
		float plrspeed = 0.0;  // Rychlost hráče
		vector plrdirection;   // Směr pohybu

		DayZInfectedInputController inputController = this.GetInputController();
		hic.GetMovement(plrspeed, plrdirection);  // Získání rychlosti a směru pohybu

		vector currentOrientation = this.GetOrientation();
		float rotationSpeed = 150.0;  // Čím vyšší hodnota, tím rychlejší rotace
		float targetYaw = currentOrientation[0];  // Počáteční yaw úhel
		if (plrdirection.Length() > 0.1)  // Pokud hráč vydá pohybový příkaz
		{
			if (this.GetCommand_Move() == null && plrdirection.Length() > 0.1)
			{
				this.StartCommand_Move();
			}

		}
		if (plrdirection[2] > 0)  // Pohyb dopředu
		{
			if (plrdirection[0] < 0)
			{
				targetYaw -= rotationSpeed * pDt * 5.0;  // Plynulá rotace vlevo
				inputController.OverrideMovementSpeed(true, 2.0);  
			}
			else if (plrdirection[0] > 0)
			{
				targetYaw += rotationSpeed * pDt * 5.0;  // Plynulá rotace vpravo
				inputController.OverrideMovementSpeed(true, 2.0);  
			}
			else
			{
				inputController.OverrideMovementSpeed(true, 2.0);  
			}
		}
		else if (plrdirection[2] < 0)  // Pohyb dozadu
		{
			inputController.OverrideMovementSpeed(true, 0);
			return true;
		}
		else  // Pokud se hráč jen otáčí na místě
		{
			if (plrdirection[0] < 0)  // Otočení vlevo
			{
				targetYaw -= rotationSpeed * pDt * 5.0;  // Plynulá rotace vlevo
			}
			else if (plrdirection[0] > 0)  // Otočení vpravo
			{
			targetYaw += rotationSpeed * pDt * 5.0;  // Plynulá rotace vpravo
			}
			else
			{
				inputController.OverrideMovementSpeed(true, 0);
				return true;  // Zůstává v aktuální orientaci
			}
		}

		// Plynulá interpolace yaw úhlu podobná free kameře
		float interpolatedYaw = Math.Lerp(currentOrientation[0], targetYaw, pDt * 9.0);  // Pomalejší přiblížení
		vector newOrientation = Vector(interpolatedYaw, currentOrientation[1], currentOrientation[2]);
		this.SetOrientation(newOrientation);  // Nastavení nové orientace

		return true;
	}

	protected bool m_IsAutoAttacking = false;  // Příznak automatického útoku
	protected float m_AutoAttackCooldown = 1.0;  // Cooldown po útoku
	protected float m_AutoAttackTimer = 0.0;  // Časovač pro sledování útoku

	// Automatický útok
	void AutoAttack(float pDt)
	{
		DayZInfectedInputController inputController = this.GetInputController();
		if (m_AutoAttackTimer > 0.0)
		{
			m_AutoAttackTimer -= pDt;
			return;  // Počkej, dokud cooldown nevyprší
		}

		//GetGame().ChatPlayer("Checking for autoattack target...");

		EntityAI target = FindNearestTarget(10.0);  // Najdi cíl v dosahu 10 metrů

		if (target && IsTargetAlive(target))
		{
		// GetGame().ChatPlayer("Target found: " + target.GetType());
			float distance = vector.Distance(this.GetPosition(), target.GetPosition());

			if (distance < 2.0)  // Blízkost cíle
			{
				if (!m_IsAutoAttacking)
				{ 	
					m_IsAutoAttacking = true;  // Nastav příznak autoattacku
				
					GetGame().ChatPlayer("Auto-attacking target: " + target.GetType());
					this.StartCommand_Attack(target, 0, 0);  // Náhodný typ útoku
					m_AutoAttackTimer = m_AutoAttackCooldown;  // Nastav cooldown po útoku
				}
			}
			else
			{
			//  GetGame().ChatPlayer("Target is out of melee range.");
				ResetAutoAttack();  // Reset, pokud je cíl mimo dosah
			}
		}
		else
		{
			ResetAutoAttack();  // Reset, pokud není žádný cíl
		// GetGame().ChatPlayer("No target found.");
		}
	}

	// Reset autoattacku
	void ResetAutoAttack()
	{
		m_IsAutoAttacking = false;  // Vypni autoattack
		m_AutoAttackTimer = 0.0;  // Resetuj časovač
	}
	// Najdi nejbližší cíl v dosahu (pouze ZombieBase nebo AnimalBase)
	EntityAI FindNearestTarget(float maxDistance)
	{
		array<Object> objectsInRadius = new array<Object>();
		GetGame().GetObjectsAtPosition3D(this.GetPosition(), maxDistance, objectsInRadius, null);

		EntityAI nearestTarget = null;
		float nearestDistance = maxDistance;

		foreach (Object obj : objectsInRadius)
		{
			if (obj == this)
			{
				continue;  // Přeskoč sebe sama
			}

			// Zkontroluj, zda objekt dědí ze ZombieBase nebo AnimalBase
			ZombieBase zombie = ZombieBase.Cast(obj);
			AnimalBase animal = AnimalBase.Cast(obj);

			if (zombie || animal)
			{
				float distance = vector.Distance(this.GetPosition(), obj.GetPosition());
				if (distance < nearestDistance)
				{
					nearestTarget = EntityAI.Cast(obj);
					nearestDistance = distance;
				}
			}
		}

		return nearestTarget;  // Vrať nejbližší cíl nebo null, pokud nebyl nalezen
	}

	void SetCreaturelCmd(int creatureCmd)
	{
		m_CreaturelCmd = creatureCmd;
		SetSynchDirty();
	}

	int GetCreaturelCmd()
	{
		return m_CreaturelCmd;
	}

	override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();
    }

	PlayerBase GetServerPlayer()
	{
		array<Man> players = new array<Man>();
		GetGame().GetPlayers(players);  // Naplní seznam připojených hráčů

		foreach (Man player : players)
		{
			PlayerBase pb = PlayerBase.Cast(player);
			if (pb)  // Pokud hráč existuje
			{
				// Můžeš zde přidat další logiku (např. výběr konkrétního hráče)
				return pb;  // Vrať hráče
			}
		}

		return null;  // Pokud není žádný hráč nalezen
	}



	//Original method functions
	// Funkce pro kontrolu objektů v okolí a aktualizaci navmesh
    void UpdateNavmeshInRadius()
    {
        float radius = 20.0;  // Radius pro kontrolu objektů kolem zombie
        vector position = this.GetPosition();
        array<Object> nearbyObjects = new array<Object>();
        array<CargoBase> proxyCargos = new array<CargoBase>();

        // Získání objektů v radiusu 20 metrů
        GetGame().GetObjectsAtPosition3D(position, radius, nearbyObjects, proxyCargos);

        // Zpráva pro hráče
        GetGame().ChatPlayer("Checking objects within radius for navmesh update...");

        // Foreach pro objekty v okolí
        for (int i = 0; i < nearbyObjects.Count(); i++)
        {
            Object obj = nearbyObjects.Get(i);
            if (obj.CanAffectPathgraph())
            {
                obj.SetAffectPathgraph(true, false);

                // Naplánování aktualizace navmesh pro objekt s 10 sekundovým zpožděním
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 10, false, obj);

                // Zpráva pro hráče o aktualizaci navmesh pro konkrétní objekt
                GetGame().ChatPlayer("Navmesh updated for object: " + obj.GetType());

                // Regenerace navmesh (zpožděný volání pomocí CallLater)
                GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateNavmesh, 500, false);  // Zpoždění 500ms
            }
        }
    }
	 // Funkce pro regeneraci navmesh
    void UpdateNavmesh()
    {
        // Zpráva pro hráče
        GetGame().ChatPlayer("Regenerating navmesh...");

        // Kód pro aktualizaci navmesh
        GetGame().UpdatePathgraphRegionByObject(this);  // Příklad volání pro aktualizaci navmesh
    }

	  override bool ResistContaminatedEffect()
    {
        return true;
    }

    override bool CanBeSkinned()
    {
        return false;
    }

    override bool IsHealthVisible()
    {
        return true;
    }

    override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
    {
        super.EEHealthLevelChanged(oldLevel, newLevel, zone);
        int currentHealthLevel;
        currentHealthLevel = GetHealthLevel();

        // IF DAMAGING

        // Health <=70% && Health >= 51%
        /*if (oldLevel == GameConstants.STATE_PRISTINE && newLevel == GameConstants.STATE_WORN) {
            GetGame().ChatPlayer("worn - health between 70% and 51%");
        }
        // Health <=50% && Health >= 31%
        if (oldLevel == GameConstants.STATE_WORN && newLevel == GameConstants.STATE_DAMAGED) {
            GetGame().ChatPlayer("moderately damaged - health between 50% and 31%");
        }
        // Health <=30% && Health >= 1%
        if (oldLevel == GameConstants.STATE_DAMAGED && newLevel == GameConstants.STATE_BADLY_DAMAGED) {
            GetGame().ChatPlayer("heavily damaged - health between 30% and 1%");
        }
        // Health <=0% --> DEATH
        // Toto by mělo kontrolovat přechod na STATE_RUINED, ne zpět na STATE_DAMAGED
        if (oldLevel == GameConstants.STATE_BADLY_DAMAGED && newLevel == GameConstants.STATE_RUINED) {
            GetGame().ChatPlayer("ruined - health at 0%");
        }

        // Kontrola, zda je nový stav STATE_RUINED
        if (newLevel == GameConstants.STATE_RUINED) {
            GetGame().ChatPlayer("ruined - health at 0%");
        }*/

    }

	 //-------------- NEW COMBAT MECHANIC (METHODS)----------------

    // Weapon DamageTypes
    /*
    DEFAULT = 0;
    SLASH = 1;
    PIERCE = 2;
    SPECIALIST = 3;
    CAVALRY = 4;
    CRUSH = 5;
    SIEGE = 6;
    FLAME = 7;
    FROST = 8;
    MAGIC = 9;
    HERO = 10;
    HERO_RANGED = 11;
    STRUCTURAL = 12;
    POISON = 13;
    */
    float GetEnemyDamageType(ZombieBase zombie)
    {
        return GetGame().ConfigGetFloat("CfgVehicles " + zombie.GetType() + " DamageType");
    }

    float GetEnemyDamageTypItem(ItemBase item)
    {
        return GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " DamageType");
    }

    float GetEnemyDamageTypItem2(ItemBase item)
    {
        return GetGame().ConfigGetFloat("CfgVehicles " + item.GetType() + " DamageType2");
    }


    float CalculateAdjustedDamage(TotalDamageResult damageResult, string dmgZone, string armorMultiplierFromConfig, float armorModifier)
    {
        float ArmorMultiplier = GetGame().ConfigGetFloat("CfgVehicles " + this.GetType() + " " + armorMultiplierFromConfig);
        float damage = damageResult.GetDamage(dmgZone, "Health");
        return damage * (ArmorMultiplier - armorModifier);
    }

    void ApplyDamage(string dmgZone, float adjustedDamage)
    {
        float newHealth = this.GetHealth(dmgZone, "Health") - adjustedDamage;
        this.SetHealth(dmgZone, "Health", newHealth);
    }

	void ~mefa_Humanoid_Logic_Base()
    {
         m_IsControlled = false;  // Odpojení orka při destrukci objektu
    }

}