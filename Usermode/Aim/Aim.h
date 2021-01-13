
/// <summary>
/// Class that holds aim functions.
/// </summary>

class AimClass {
public:

	/// <summary>
	/// Aimbot is a function that gets player info such as 
	/// position, health, team, view angles. The goal of this
	/// function is to collect player info and do math to set
	/// local player view angles to aim at a player. The player
	/// we will aim at is decided by the funcion GetNearestEntity
	/// which returns the entity closest to the local players
	/// crosshair.
	/// </summary>
	
	void Aimbot(bool Status);

	/// <summary>
	/// Uses same infomation as aimbot. Refer to aimbot summary.
	/// Intead of using GetNearestEntity function to find the
	/// player we will aim at, we loop through all valid entitys
	/// and aim at each of tem in a constant loop.
	/// </summary>

	void EntitySwap(bool Status);

	/// <summary>
	/// As the name suggests, it rotates your gun in a matter to
	/// direct it tward an enemy. Uses same infomation as aimbot.
	/// Refer to aimbot summary. Using GetNearestEntity we find 
	/// the nearest entity and do some quick maths to change gun
	/// rotation. Also referd to as silent aim.
	/// </summary>

	void GunRotation(bool Status);

	/// <summary>
	/// Simular to gunrotation, but instead of rewriting the local
	/// gun angle we change the bullet angle.
	/// </summary>
	
	void Perfectsilent(bool Status);

};

