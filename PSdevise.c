// gcc -o PSdevise PSdevise -lvlc

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <vlc/vlc.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

//Khai bao ham
void updateRequest();
void mainProcess(int , int *);
int getQueue(int queue[], int* );
int putQueue(int , int queue[], int* );

//bien toan cuc
int position;
int queue1[10], queue25[10];
int queue1_size = 0;
int queue25_size = 0;
int request1 = 0, request25 = 0;
int request = 0;

void openthedoor(){
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;
	inst = libvlc_new(0, NULL);
	m = libvlc_media_new_path(inst, "open_the_door.mp3");
	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_release(m);
	libvlc_media_player_play(mp);
	usleep(2000000);
    // stop playing
	libvlc_media_player_stop(mp);
    // free the memory.
	libvlc_media_player_release(mp);
	libvlc_release(inst);
}
void closethedoor(){
	libvlc_instance_t *inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;
	inst = libvlc_new(0, NULL);
	m = libvlc_media_new_path(inst, "close_the_door.mp3");
	mp = libvlc_media_player_new_from_media(m);
	libvlc_media_release(m);
	libvlc_media_player_play(mp);
    usleep( 1500000); // let it play for 10 seconds
    // stop playing
    libvlc_media_player_stop(mp);
    // free the memory.
    libvlc_media_player_release(mp);
    libvlc_release(inst);
}

int main(){

	//Create shared memory
	int *shm, shmid;
	key_t key;
	key = 1234;
	shmid = shmget(key, 128, IPC_CREAT|0666);
	shm = (int *)shmat(shmid, NULL, 0);

	
//Khoi tao cac gia tri ban dau trong shm
	shm[0] = 1;  //Vi tri hien tai
	shm[1] = 0;
	shm[2] = 0;


//shm[0]: vi tri thang may
//shm[1]: yeu cau gui do tu tang 1
//shm[2]: yeu cau gui do tu tang 2->5

//Vong while chay den khi nao gap tin hieu ket thuc


	while(1){

		updateRequest(shm);
		request1 = getQueue(queue1, &queue1_size);	
		request25 = getQueue(queue25, &queue25_size);	
		
		if(request1 != 0)
			mainProcess(request1, shm);
		if(request25 != 0)
			mainProcess(request25, shm);
	}
	return 0;
}

//Ham cap nhat yeu cau 
void updateRequest(int *shm){
	if(shm[1] != 0){
		request = shm[1];
		putQueue(request, queue1, &queue1_size);
		shm[1] = 0;
	}
	if(shm[2] != 0){
		request = shm[2];
		putQueue(request, queue25, &queue25_size);
		shm[2] = 0;
	}
}

//Ham xu ly
void mainProcess(int request, int *shm){
	if(request == 0){
		return;
	}
//mac dinh dang o tang 1
//do cao tu tang 1
	int high = (request - 1)*3;
//thoi gian chay
	int time = (int)high/0.5;
	printf("thang may dang di len trong thoi gian %ds\n", time);
	for(int i = 1; i <= time; i++){
		printf("thang may dang o do cao %0.1fm\n", 0.5*i);
		int a = (int)(0.5*i);
		if(a%3 == 0){
			shm[0] = (0.5*i)/3 + 1;
			// printf("Thang may dang o tang %d\n", shm[0]);
		}
		sleep(1);

	}
	printf("Thang may da dung\n");
	printf("Mo Cua\n");
	
	printf("....\n");
	// openthedoor();
	
	printf("....\n");
	
	// closethedoor();
	printf("Dong cua\n");
	printf("Thang may dang di xuong\n");
	for(int i = 0; i<time; i++){
		printf("thang may dang o do cao %0.1fm\n", (float)(high - 0.5*i));
		int a = (int)(high - 0.5*i);
		if(a%3 == 0){
			shm[0] = a/3 + 1;
			// printf("Thang may dang o tang %d\n", shm[0]);
		}
		sleep(1);
	}
	printf("Thang may da dung\n");
	sleep(3);
}


//Chu y size can truyen con tro vao vi gia tri can thay doi
int getQueue(int queue[], int *size){
	int result;
	if(*size == 0){
		return 0;
	}else{
		result = queue[0];
		for(int i = 0; i<*size; i++){
			queue[i] = queue[i+1];
		}
		*size = *size-1;
	}
	return result;
}

int putQueue(int a, int queue[], int *size){
	for(int i = 0; i<*size; i++){
		if(queue[i] == a){
			return 0;
		}
	}
	queue[*size] = a;

	*size = *size+1;
}
