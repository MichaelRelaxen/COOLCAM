#include "pad.h"
#include "types.h"

/* externs */
extern int32_t cellPadGetData(uint32_t port_no, cellPadData *data);
extern void sprintf(char *dst, char *fmt, ...);
void memcpy(void* dst, void* src, int size);
extern Vec4 player_coords;
extern int ui_toggle;
extern Moby* moby_table;
extern Moby* moby_table_top;
void memset(void *ptr, int x, uint32_t n);
extern char occlusion[];

#define NO_INPUT 0
#define NEUTRAL 0x80
#define DEADTHRESH 10

typedef struct {
	Vec4 forward;
	Vec4 right;
	Vec4 up;
	Vec4 pos;
} Camera;

typedef struct {
	int saved;
	Camera cam;
	float yaw;
	float pitch;
} Savepos;


Vec4 scalar_multiplication(float scale, Vec4 v){
	v.x = v.x*scale;
	v.y = v.y*scale;
	v.z = v.z*scale;
	
	return v;
}

Vec4 vec_add(Vec4 v, Vec4 u){
	Vec4 result;
	result.x = v.x + u.x;
	result.y = v.y + u.y;
	result.z = v.z + u.z;
	
	return result;
}

void vecclear(Vec4 *v){
	v->x = 0;
	v->y = 0;
	v->z = 0;
	v->w = 0;
}

void vecassign(Vec4 to, Vec4 from){
	to.x = from.x;
	to.y = from.y;
	to.z = from.z;
	to.w = from.w;
}
static float PI = 3.14159f;
extern Camera* camera;

float power (float x, int y){
	float sum = x;
	if (y < 0){
		for (int i = -1; i > y; i--){
			sum = sum * x;
		}
		sum = 1.0f / sum;
	}
	else{
		for (int i = 1; i < y; i++){
			sum = sum * x;
		}
	}
	
	return sum;
}

int factorial(int x){
	int sum = 1;
	for (int i = 1; i <= x; i++){
		sum *= i;
	}
	return sum;
}

float sin(float v){
	while (v > PI) v -= 2.0f * PI;
	while (v < -PI) v += 2.0f * PI;

	float x,y,sum = 0.0f;
	int z = 0;
	for (int i = 1; i < 11; i += 2){
		y = power(v, i);
		z = factorial(i);
		x = y / (float)z;
		if (((i - 1) / 2) % 2 != 0){
			sum = sum - x;
		}
		else{
			sum = sum + x;
		}
	}	
	return sum;
}

float cos(float v){
	return sin(PI / 2.0f - v);
}

Vec4 cross(Vec4 a, Vec4 b) {
	Vec4 result;
	result.x = (a.y * b.z - a.z * b.y);
	result.y = (a.z * b.x - a.x * b.z); 
	result.z = (a.x * b.y - a.y * b.x);
	return result;
}

float dot(Vec4 a, Vec4 b){
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float sqrt(float x) {
    if (x <= 0.0f) return 0.0f;
    float guess = x;
    for (int i = 0; i < 10; i++) {
        guess = (guess + x / guess) * 0.5f;
    }
    return guess;
}

float length(Vec4 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec4 normalize(Vec4 v) {
	return scalar_multiplication((1.0f / length(v)), v);
}

void identity(Vec4 v) {
	v.x = 1;
	v.y = 1;
	v.z = 1;
	v.w = 0;
}

void lookat(Vec4 pos){
	camera->forward.x = -(camera->pos.x - pos.x);
	camera->forward.y = -(camera->pos.y - pos.y);
	camera->forward.z = -(camera->pos.z - pos.z);
	camera->forward = normalize(camera->forward);
	Vec4 worldup;
	worldup.x = 0;
	worldup.y = 0;
	worldup.z = 1;
	worldup.w = 0;		
	camera->right = normalize(cross(worldup, camera->forward));
	camera->up = normalize(cross(camera->forward, camera->right));
}

static float yaw = 0.0f;
static float pitch = 0.0f;
Vec4 lookatpos;
//Moby* mobytolookat;

//KEEP FRICTION BETWEEN -1 <-> 0 OR YOU WILL ACCELERATE TOO FAST AND CRASH
// -1 = MAXFRICTION, 0 = NO FRICTION;
static float airfriction; 
static float turnfriction; 
static float turnrange = 90.0f;
Vec4 acceleration;
static float yawacceleration = 0;
static float yawvelocity = 0;
static float pitchacceleration = 0;
static float pitchvelocity = 0;
Vec4 velocity;
static float rotspeed;
static float movespeed;
int timer;
static int freecamenabled = 0;
static int lookatratchetenabled = 0;
static int init = 0;
Vec4 worldup;
Savepos savepos;
int enablemod;

void setlookatpos(){
	lookatpos.x = camera->pos.x + camera->forward.x;
	lookatpos.y = camera->pos.y + camera->forward.y;
	lookatpos.z = camera->pos.z + camera->forward.z;
	lookatpos.w = 0;
}

int32_t pad_redirect(uint32_t port_no, cellPadData *data) {
	int32_t ret = cellPadGetData(port_no, data);
	if (timer > 0)
		timer--;
		
	
	if(data->BTN_SELECT && timer == 0){
		timer = 20;
		freecamenabled = 1;
		lookatratchetenabled = 0;
		enablemod = !enablemod;
	}
	data->BTN_SELECT = NO_INPUT;
	if(!enablemod){
		ui_toggle = 0;
		return ret;
	}
	else {
		ui_toggle = 1;
	}
	
	if (ret != 0 || data->len == 0)
		return ret;
	memset(occlusion, 0xff, 0xff);
	if(!init){
		identity(acceleration);
		identity(velocity);
		identity(lookatpos);
		worldup.x = 0;
		worldup.y = 0;
		worldup.z = 1;
		worldup.w = 0;
		rotspeed = 0.03f;
		movespeed = 0.05f;
		airfriction = -0.1f;
		turnfriction = -0.1f;
		init = 1;
	}

	if (data->BTN_START && timer == 0){
		timer = 20;
		freecamenabled = !freecamenabled;
	}
	
	if (data->BTN_R2 && timer == 0){
		timer = 20;
		lookatratchetenabled++;
		if (lookatratchetenabled > 2)
			lookatratchetenabled = 0;
	}
	
	if(data->BTN_SQUARE){
		savepos.cam = *camera;
		savepos.saved = 1;
		savepos.yaw = yaw;
		savepos.pitch = pitch;
	}
	
	if(data->BTN_CIRCLE){
		if (savepos.saved){
			*camera = savepos.cam;
			yaw = savepos.yaw;
			pitch = savepos.pitch;
			pitchvelocity = 0;
			yawvelocity = 0;
			vecclear(&velocity);
		}
	}
	
	float y = yaw * (PI / 180.0f);
	float p = pitch * (PI / 180.0f);
	
	float cp = cos(p);
	float sp = sin(p);
	float cy = cos(y);
	float sy = sin(y);
	
	camera->forward.x = cy * cp;
	camera->forward.y =	sy * cp;
	camera->forward.z =	-sp;
	
	camera->right.x = -sy;
	camera->right.y = cy;
	camera->right.z = 0.0f;
	
	camera->up.x = cy * sp;
	camera->up.y = sy * sp;
	camera->up.z = cp;
	
	if(data->BTN_R3 && !lookatratchetenabled ){
		setlookatpos();
	}
	
	switch(lookatratchetenabled){
		case 1: 
			if (lookatpos.x == 0.0f)
				setlookatpos();
			lookat(lookatpos);	
			break;
		case 2:
			lookat(player_coords);
			break;
	}
	
	if(freecamenabled){
		//RESET ACCELERATION SO IT DOESNT COMPOUND ENDLESSLY
		acceleration.x = 0;
		acceleration.y = 0;
		acceleration.z = 0;
		acceleration.w = 0;
		pitchacceleration = 0;
		yawacceleration = 0;
		
		//INTRODUCE FRICTION
		pitchvelocity = pitchvelocity + pitchvelocity * turnfriction; 
		yawvelocity = yawvelocity + yawvelocity * turnfriction;
		velocity = vec_add(velocity, scalar_multiplication(airfriction, velocity));
		
		//Direction to move camera forward in the world regardless of camera pitch.
		Vec4 coolforwarddir;
		coolforwarddir = normalize(cross(camera->right, worldup));
		
		
		//ADD TO ACCELERATION WITH INPUTS
		yawacceleration += ((128 - data->ANA_R_H) / DEADTHRESH) * rotspeed;
		pitchacceleration -= ((128 - data->ANA_R_V) / DEADTHRESH) * rotspeed;
		if ((128 - data->ANA_L_H) > DEADTHRESH || (128 - data->ANA_L_H)  < -DEADTHRESH)
			acceleration = vec_add(acceleration, scalar_multiplication(((float)(128 - data->ANA_L_H)) * movespeed / 128.0f, camera->right));
		if ((128 - data->ANA_L_V) > DEADTHRESH || (128 - data->ANA_L_V)  < -DEADTHRESH)
			acceleration = vec_add(acceleration, scalar_multiplication(((float)(128 - data->ANA_L_V)) * movespeed / 128.0f, camera->forward));
		if (data->BTN_L1)
			acceleration.z += movespeed;
		if (data->BTN_L2)
			acceleration.z -= movespeed;
		if (data->BTN_UP)
			acceleration = vec_add(acceleration, scalar_multiplication(movespeed, coolforwarddir));
		if (data->BTN_DOWN)
			acceleration = vec_add(acceleration, scalar_multiplication(-movespeed, coolforwarddir));
		if (data->BTN_LEFT)
			acceleration = vec_add(acceleration, scalar_multiplication(movespeed, camera->right));
		if (data->BTN_RIGHT)
			acceleration = vec_add(acceleration, scalar_multiplication(-movespeed, camera->right));
		
		//FLY WITH PLAYER
		if(data->BTN_R1 && !lookatratchetenabled){
			player_coords = vec_add(camera->pos, scalar_multiplication(5.0f, camera->forward));
			data->BTN_L2 = 1;
		}	
		
		//INCREASE VELOCITY WITH ACCELERATION
		pitchvelocity = pitchvelocity + pitchacceleration;
		yawvelocity = yawvelocity + yawacceleration;
		velocity = vec_add(velocity, acceleration);
		
		//MOVE WITH VELOCITY
		yaw += yawvelocity;
		pitch += pitchvelocity;
		camera->pos = vec_add(camera->pos, velocity);
		
		//LIMITS TURN RANGE TO 90 DEGREES UP AND DOWN
		if (pitch > turnrange)
			pitch = turnrange;
		else if (pitch < -turnrange)
			pitch = -turnrange;
	}
	
	data->BTN_SELECT = NO_INPUT;
	data->BTN_START = NO_INPUT;
	data->BTN_R3 = NO_INPUT;
	data->BTN_UP = NO_INPUT;
	data->BTN_DOWN = NO_INPUT;
	data->BTN_LEFT = NO_INPUT;
	data->BTN_RIGHT = NO_INPUT;
	if(freecamenabled){
		data->BTN_L1 = NO_INPUT;
		data->ANA_L_H = NEUTRAL; 
		data->ANA_L_V = NEUTRAL;
	}
    data->ANA_R_H = NEUTRAL; 
    data->ANA_R_V = NEUTRAL;
	
	//¯\_( ͡° ͜ʖ ͡°)_/¯
	return ret;
}