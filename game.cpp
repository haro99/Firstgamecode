////////////////////////////////////////////////////////////////////
// �ǂ����������v���O����
////////////////////////////////////////////////////////////////////

// �C���N���[�h -------------------------------------
#define _USE_MATH_DEFINES
#define HEIGHT 480		//�E�B���h�E�̏c�̒���
#define WIDTH 640		//�E�B���h�E�̕��̒���
#define SHIPLENGTH 64	//�D�̏c���̒���
#define TARGETX	600		//�I�̊���̈ʒu
#define TARGETY 64		//�I�̏c�̒���
#define ARROWLENGTH 16	//��̉��̒���
#define ARROWWIDTH 8	//��̏c�̕�

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "gs.h"


// �Q�[�����[�h�p
enum
{
	Title = 0,	//�^�C�g�����
	Setting = 1,	//�D�̐ݒ�
	Update = 2,	//��̍X�V
	Result = 3,	//����
	Over = 4,	//�I�[�o�[���
};
enum
{
	YSET		= 0,
	ANGLESET	= 1,
	POWERSET	= 2,
};
enum
{
	OFF = 0,
	ON	= 1,
};
// �֐��̐錾 -----------------------------
void GameInit(void);	// �Q�[��������
void GameLoop(void);	// �Q�[�����[�v
void GameTerm(void);	// �Q�[���I��
void Playerset(void);	//�v���C���[�̍X�V
void Arrowupdate(void);	//��΂�����̍X�V
void Setreset(void);	//�D�A�I�̃��Z�b�g
bool Numbercheck(int count[], int i, int number);	//�I�������̂����邩�`�F�b�N
void Pointdisplay();	//�|�C���g�\��
void TitleMain();		//�^�C�g��
void OverMain();		//�I�[�o�[

// �G�p�̕ϐ�
LPDIRECT3DTEXTURE9	sBack;		// �w�i
LPDIRECT3DTEXTURE9	sBack2;		// �w�i2
LPDIRECT3DTEXTURE9	sBack3;		// �w�i3
LPDIRECT3DTEXTURE9	sTitle1;		// �^�C�g��1
LPDIRECT3DTEXTURE9	sTitle2;		// �^�C�g��2
LPDIRECT3DTEXTURE9	sOver;		// �Q�[���I�[�o�[
LPDIRECT3DTEXTURE9	sShip;		// �V�b�v
LPDIRECT3DTEXTURE9	sTarget;	// �I
LPDIRECT3DTEXTURE9	sArrow;		// ��
LPDIRECT3DTEXTURE9	sGrp1;		// �O���t1
LPDIRECT3DTEXTURE9	sGrp2;		// �O���t2

// �O���[�o���ϐ�
int status;						//�Q�[���̏��
int shipsetting;					//�D�̐ݒ���
int shipy;						//�D�̈ʒu
double charge;					//��̊�{�p���[
float powerx, powery;			//��̍X�V�̂w�Ƃx
float arrowx, arrowy;			//��̈ʒu
float targety[3];				//�I�̔z��
int gHitKey;					//�L�[�̏��
double shipangle;				//�D�̊p�x	
double arrowangle;				//��̊p�x
int point;						//���_
int score;
int displaytime;				//�\������
int bullet;						//��̎c��


// ----------------------------------------------------------
// �Q�[���@�O���� 
// �v���O�����J�n���ɂP��̂݃V�X�e������Ă΂�� 
// ----------------------------------------------------------
void GameInit(void)
{
	//�G�̓ǂݍ���
	gsLoadTexture(&sBack,"bmp//bg.bmp", 0xff000000);		//��O�̊G
	gsLoadTexture(&sBack2, "bmp//bg2.bmp", 0xff000000);		//�^�񒆂̊G
	gsLoadTexture(&sBack3, "bmp//bg3.bmp", 0xff000000);		//��Ԍ��̊G
	gsLoadTexture(&sTitle1, "bmp//title1.bmp", 0xff000000);	//�^�C�g��1�̊G
	gsLoadTexture(&sTitle2, "bmp//title2.bmp", 0xff000000);	//�^�C�g��2�̊G
	gsLoadTexture(&sOver, "BMP/over.bmp", 0xff000000);		// �Q�[���I�[�o�[
	gsLoadTexture(&sShip, "bmp//ship.bmp", 0xff000000);		//�v���C���[�̑D
	gsLoadTexture(&sTarget, "bmp//target.bmp", 0xffffffff);	//�@�I	
	gsLoadTexture(&sArrow, "bmp/arrow.bmp", 0xff000000);	//	��
	gsLoadTexture(&sGrp1, "bmp/grp1.bmp", 0xff000000);		// ���[�^�[�P�i�ԁj
	gsLoadTexture(&sGrp2, "bmp/grp2.bmp", 0xff000000);		// ���[�^�[�Q�i�΁j

	//�ϐ��̏�����
	status = 0;
	shipsetting = 0;
	shipy = 300;
	shipangle = 0;
	gHitKey = 0;
	powerx = 1;
	charge = 0.2f;
	arrowx = 0;
	arrowy = 0;
	point = 10;
	score = 0;
	bullet = 3;
	int count[3];
	int t = (int)time(0);
	srand(t);
	//�I�̏�����3��
	for (int i = 0; i < 3;)
	{
		int number = rand() % 7;
		//���܂łɏo�Ă���ŏ��ɖ߂�
		if (true == Numbercheck(count, i, number)) continue;
		count[i] = number;
		targety[i] = (float)64 * number;
		i++;
	}

}

// ----------------------------------------------------------
// �Q�[���@���[�v����
// �����[�v���ƂɃV�X�e������Ă΂��
// ----------------------------------------------------------
void GameLoop(void)
{

	// �w�i��`��
	gsDraw2D(sBack3, 0, 0);
	gsDraw2D(sBack2, 0, 0);
	gsDraw2D(sBack, 0, 0);


	//���[�h
	switch (status)
	{
		case Title:
			TitleMain();
			break;
		case Setting:
			Playerset();
			break;
		case Update:
			Arrowupdate();
			break;
		case Result:
			Pointdisplay();
			break;
		case Over:
			OverMain();
			break;
	}

	// �L�[�𗣂������̃`�F�b�N
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) == 0)
	{
		gHitKey = 0;
	}
	if (status == 1 || status == 2 || status == 3)
	{
		//�D�̕\��
		gsDraw2DPartEx(sShip, 100, shipy, SHIPLENGTH, SHIPLENGTH, 0, 0, 64, 64, (float)shipangle, 0xffffffff);

		//�I�̕\��
		for (int i = 0; i < 3; i++)
		{
			gsDraw2D(sTarget, TARGETX, (int)targety[i]);
		}
		//	��̕\��
		if (status == Update || status == Result)
		{
			arrowangle = atan2(powery, powerx) * -1;
			gsDraw2DPartEx(sArrow, (int)arrowx, (int)arrowy, 16, 8, 0, 0, 16, 8, (float)arrowangle, 0xffffffff);
		}


		// �^�C�g����`��
		gsDrawText(10, 10, "�I����");

		//// �F�X�ȃX�e�[�^�X��`��
		//gsDrawNum(300, 10, shipy);
		//gsDrawNum(300, 30, shipangle * 180 / M_PI);	//���ˊp�x(�x)
		//gsDrawNum(300, 50, arrowangle);
		//gsDrawNum(300, 70, powery);
		//gsDrawNum(300, 90, score);

		//��̃p���[�̃��[�^�[�\��
		gsDraw2DEx(sGrp1, 100, 20, 10 * 10, 16); 	// ��
		gsDraw2DEx(sGrp2, 100, 20, powerx * 10, 16, 0xff007700);	// ��

		int ypoint = 50;

		//��̎c��̕\��
		for (int i = 0; i < bullet; i++)
		{
			gsDraw2D(sArrow, 30, ypoint);
			ypoint += 20;
		}
	}
}

// ----------------------------------------------------------
// �Q�[���@�㏈��
// �v���O�����I�����ɂP��̂݃V�X�e������Ă΂��
// ----------------------------------------------------------
void GameTerm(void)
{
	// �G�̉��
	gsReleaseTexture(sBack);
	gsReleaseTexture(sBack2);
	gsReleaseTexture(sBack3);
	gsReleaseTexture(sTitle1);
	gsReleaseTexture(sShip);
	gsReleaseTexture(sArrow);
	gsReleaseTexture(sGrp1);
	gsReleaseTexture(sGrp2);


}
// ----------------------------------------------------------
// �v���C���[�̍X�V
// ----------------------------------------------------------
void Playerset(void)
{
	//�D�̍�����ݒ�(y���W�j
	switch (shipsetting)
	{
	case YSET:
		//��֏㏸
		if ((GetAsyncKeyState(VK_UP) & 0x8000) != 0)
		{
			shipy -= 5;
		}
		//���։��~
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000) != 0)
		{
			shipy += 5;
		}
		//�͂ݏo���h�~
		if (shipy < 100 + SHIPLENGTH)
		{
			shipy = 100 + SHIPLENGTH;
		}
		if (shipy > HEIGHT)
		{
			shipy = HEIGHT;
		}
		//�X�y�[�X�������č��������߂����H
		if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			shipsetting = 1;			//ANGLESET
			gHitKey = 1;				//�L�[��������
		}
		break;
	//�D�̊p�x��ݒ�(���W�A��)
	case ANGLESET:
		//1���W�A�����ƂɊp�x��ω�������
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) != 0)
		{
			shipangle += 1 * M_PI / 180;
		}
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0)
		{
			shipangle -= 1 * M_PI / 180;
		}
		//�O�x�ȉ��ɂȂ�Ƃ��͂O�x�ɂɌŒ�
		if (shipangle < 0)
		{
			shipangle = 0;
		}
		//�L�[����x�����Ă���X�y�[�X�����������H
		if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			shipsetting = 2;			//POWERSET��
			gHitKey = 1;				//�L�[��������
		}
		break;
	//��̐i�ޗ͂�ݒ�ix���W�j
	case POWERSET:
		//�p���[���`���[�W���Ă���
		powerx += charge;
		//�p���[���K��𒴂�����`���[�W�𔽓]
		if (powerx < 1 || powerx>10)
		{
			charge *= -1;
		}
		//�L�[����x�b���Ă���X�y�[�X�������ꂽ���H
		if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
		{
			//��̔��ˈʒu����
			arrowx = 100 + 32;
			arrowy = shipy - 10;
			powery = powerx * tan(shipangle) * -1;	//�E������ɔ��ōs���̂Ń}�C�i�X�������Ēu��
			gHitKey = 1;							//�L�[��������
			status = 2;								//update��
		}
		break;
	}
}

// ----------------------------------------------------------
// ��΂�����̍X�V
// ----------------------------------------------------------
void Arrowupdate()
{
	//��̍X�V
	arrowx += powerx;
	powery += 0.08f;	//�d�͂𑫂��Ă���
	arrowy += powery;

	//�E�B���h�E�O�ɔ��ōs�����烊�Z�b�g����
	if (WIDTH < arrowx || HEIGHT < arrowy)
	{
		point = 0;
		bullet--;				//��̎c������炷
		status = 3;				//���ʂ�
	}

	//�I�̐��������[�v
	for (int i = 0; i < 3; i++)
	{
		//�I�Ɏh�����Ă邩�H
		if (TARGETX + 12 < arrowx + ARROWLENGTH&&
			TARGETX + 20 > arrowx + ARROWLENGTH&&
			targety[i]<arrowy + (ARROWWIDTH / 2) &&
			targety[i] + TARGETY>arrowy + (ARROWWIDTH / 2))
		{
			point = 10;		//���_
			status = 3;			//���ʂ�
			break;
		}
	}
}
// ----------------------------------------------------------
// �D�A�I�̃��Z�b�g
// ----------------------------------------------------------
void Setreset()
{
	//���l��������
	status = 1;
	shipsetting = 0;
	arrowx = 0;
	arrowy = 0;

	int count[3];
	//�����������o�Ă���ŏ��ɖ߂�
	for (int i = 0; i < 3;)
	{
		int number = rand() % 7;
		//�������������������H
 		if (true == Numbercheck(count, i, number)) continue;
		//�����ɓ���Ă���
		count[i] = number;
		//�I�̈ʒu����
		targety[i] = (float)64 * number;
		i++;
	}
}
// ----------------------------------------------------------
// �I�������̂����邩�`�F�b�N
// ----------------------------------------------------------
bool Numbercheck(int count[], int i, int number)
{
	for (int j = 0; j < i; j++)
	{
		if (count[j] == number)
		{
			return true;
		}
	}
	return false;
}
// ----------------------------------------------------------
// �|�C���g�\��
// ----------------------------------------------------------
void Pointdisplay()
{
	if (displaytime < 180)
	{
		gsDrawNum(WIDTH/2, HEIGHT/2, point);
		displaytime++;
	}
	else
	{
		displaytime = 0;
		score += point;

		//��̎c�肪���邩�H
		if (0 < bullet)
		{
			Setreset();
		}
		//�Ȃ��ꍇ
		else
		{
			status = 4;		//�Q�[���I�[�o�[��
		}
	}
}
// ----------------------------------------------------------
// �^�C�g���̕\���E�X�V
// ----------------------------------------------------------
void TitleMain()
{
	//�^�C�g��1
	gsDraw2D(sTitle1, 90, 50);
	//
	gsDraw2D(sTitle2, 90, 150);
	
	//�L�[����x�b���Ă���X�y�[�X�������ꂽ���H
	if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
	{
		gHitKey = 1;	//�L�[��������
		status = 1;		//�v���C���[�X�V��
	}
}
// ------------------------------------------------ 
// �Q�[���I�[�o�[�̕\���E�X�V
// ------------------------------------------------ 
void OverMain()
{
	//�Q�[���I�[�o�[�̕\��
	gsDraw2D(sOver, 90, 50);
	//�ŏI�X�R�A��\��
	gsDrawNum(WIDTH / 2, HEIGHT / 2, point);

	//�L�[����x�b���Ă���X�y�[�X�������ꂽ���H
	if (gHitKey == OFF && (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
	{
		GameInit();		//���ׂĂ�������
		gHitKey = 1;	//�L�[��������
		status = 0;		//�^�C�g����
	}
}