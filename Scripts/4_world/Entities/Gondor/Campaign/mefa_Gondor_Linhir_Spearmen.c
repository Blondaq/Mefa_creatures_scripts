class mefa_Gondor_Linhir_Spearmen : mefa_Humanoid_Logic_Base {

	protected float         m_Scale = 0.9;
	void mefa_Gondor_Linhir_Spearmen()
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

	void ~mefa_Gondor_Linhir_Spearmen()
    {
       
    }


}