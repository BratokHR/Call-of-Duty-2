#define	MAX_EDIT_LINE				256
#define MAX_ENTITIES_IN_SNAPSHOT	256
#define MAX_PLAYERS					64

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

typedef enum {
	qfalse,
	qtrue
} qbool;

typedef enum {
	TEAM_FREE,
	TEAM_AXIS,
	TEAM_ALLIES,
	TEAM_SPECTATOR,
	TEAM_NUM_TEAMS
} team_t;

typedef struct {
	qbool		infoValid;
	qbool		unknown;
	int			clientNum;
	char		name[32];
	team_t		team; 
	int			pad[4];
	char		bodymodel[64];
	char		headmodel[64];
	char		helmetmodel[64];

	int			pad1[203];
	int *		pointer;
	int			pad2[11];

	int			movement;

	int			pad3[3];

	qbool		usingMountedWeapon;

	int			pad4;

	int			pose;

	int			pad5[11];

	int *		unknown_ptr;
	int			curWeapon;	

	int			pad6[3];
} clientInfo_t; //size=1208, dw=302

typedef struct {
	int				x;
	int				y;
	int				width;
	int				height;
	float			fov_x;
	float			fov_y;
	vec3_t			vieworg;
	vec3_t			viewaxis[3];
	int				time;
	int				rdflags;
	BYTE			areamask[8]; //?
} refdef_t; //size=88, dw=22

typedef struct {
	int			commandTime;	// cmd->serverTime of last executed command
	int			pm_type;
	int			bobCycle;		// for view bobbing and footstep generation
	int			pm_flags;		// ducked, jump_held, etc
	int			pm_time;
	vec3_t		origin;
	vec3_t		velocity;

	int			padding00[31];

	int			clientNum;

	int			pading01[2422];
} playerState_t; //size=10404, dw=2465

typedef struct {
	int		trType;
	int		trTime;
	int		trDuration;
	vec3_t	trBase;
	vec3_t	trDelta;
} trajectory_t;

typedef struct {
	int				number;
	int				eType;
	int				eFlags;
	trajectory_t	pos;
	trajectory_t	apos;

	int				pad[8];

	int				otherEntityNum;
	int				otherEntityNum2;

	int				pad4[5];

	int				clientNum;

	int				pad2[2];

	int				solid;
	int				eventParm;

	int				pad5[9];

	int				weapon;
	int				legsAnim;
	int				torsoAnim;

	int				pad3[7];
} entityState_t; //size=240, dw=60

typedef struct {
	int				padding00[12];

	playerState_t	ps;
	int				numEntities;

	int				padding01;

	entityState_t	entities[ MAX_ENTITIES_IN_SNAPSHOT ];
} snapshot_t;

typedef struct {
	int				clientFrame;
	int				clientNum;

	int				padding[7];

	snapshot_t *	snap;
	snapshot_t *	nextSnap;

	int				padding0[38624];

	int				frametime;
	int				time;
	
	int				padding05[4];

	playerState_t	predictedPlayerState;
	
	int				padding5[202];

	//41308
	refdef_t		refdef; //22
	vec3_t			refdefViewAngles;

	int				padding2[3582];
	int				crosshairClientNum;
	int				crosshairClientTime;
	int				padding3[3];
	int				crosshairClientHealth; //?
	int				padding4[173];

	vec3_t			kick_angles;
	int				padding1[205375];

	//int				padding1[209139];
} cg_t; //size=1001888, dw=250472

typedef struct {
	int		cursor;
	int		widthChars1;
	int		widthChars2;
	int		pad[3];
	char	buffer[MAX_EDIT_LINE];
} mfield_t;

typedef struct {
	mfield_t	console;
	int			pad[2];
	mfield_t	chat;
} field_t;

typedef struct {
	int	 Width; 
	int	 Height; 
	int	 pad1; 
	int	 CountCommands; 
	int	 FrameCount; 
	int	 pad2; 
	char GameType[32]; 
	char ServerName[256]; 
	int	 MaxPlayers; 
	char MapName[256]; 
} serverInfo_t;

struct client_t {
	bool	readCommands;
	bool	onVodka;

	int		readTick;

	bool	spec;
	int		spec_id;
};

std::string args_list[7] = { "player", "player id", "message", "ip | login", "min max", "team", "mapname" };