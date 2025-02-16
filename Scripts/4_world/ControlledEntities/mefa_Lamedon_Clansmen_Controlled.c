class mefa_Lamedon_Clansmen_Controlled : mefa_Humanoid_Logic_Base {

	protected ref LamedonClansmenControlledCommandScriptTable commandTable = new LamedonClansmenControlledCommandScriptTable(this);
	protected float         m_Scale = 0.9;
	void mefa_Lamedon_Clansmen_Controlled()
	{
		SetEventMask(EntityEvent.POSTFRAME);
		RegisterNetSyncVariableFloat("m_Scale");
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

	
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);
		int currentHealthLevel;
		currentHealthLevel = GetHealthLevel();
		LamedonClansmenControlledArmorTestCommandScript cmdArmorTest = new LamedonClansmenControlledArmorTestCommandScript(this, commandTable);
		

		// Health <=50% && Health >= 31%
		if (oldLevel == GameConstants.STATE_WORN && newLevel == GameConstants.STATE_DAMAGED) {
			GetGame().ChatPlayer("moderately damaged - health between 50% and 31%");
			StartCommand_Script(cmdArmorTest);
			armorModifier = 0.4;
			GetGame().ChatPlayer("+ 40% armor activated ");
		}



	}

	void ~mefa_Lamedon_Clansmen_Controlled()
    {
       
    }

	
}


/**
 * Custom animation graph commands and variables
 */
class LamedonClansmenControlledCommandScriptTable
{
	int armorTestVar;

	/**
	 * Bind variables and commands
	 */
	void LamedonClansmenControlledCommandScriptTable(mefa_Lamedon_Clansmen_Controlled pDZCreature)
	{
		DayZCreatureAnimInterface dzcai = pDZCreature.GetAnimInterface();

		armorTestVar = dzcai.BindVariableBool("armorTest");
	}
}

/**
 * Custom command to force giant into injured state
 */
class LamedonClansmenControlledArmorTestCommandScript extends DayZInfectedCommandScript
{
	protected LamedonClansmenControlledCommandScriptTable commandTable;

	void LamedonClansmenControlledArmorTestCommandScript(DayZInfected pInfected, LamedonClansmenControlledCommandScriptTable table)
	{
		commandTable = table;
	}

	/**
	 * Set injured varibale to switch walking animation
	 */
	override void OnActivate()
	{
		PreAnim_SetBool(commandTable.armorTestVar, true);
	}

	/**
	 * Instantly finish command
	 */
	override bool PostPhysUpdate(float pDt)
	{
		return false;
	}
}