#ifndef _MINEDEF
#define _MINEDEF

//���º����ڶ��崰������ֱ߿�Ĵ�С
#define DEFAULT_FRAME_X		6
#define DEFAULT_FRAME_Y		44

#define LINE_WIDTH_0		3
#define LINE_WIDTH_1		2

#define SIDE_WIDTH_0		6
#define SIDE_WIDTH_1		5

#define SHELL_S_H			37
#define SHELL_S_START_X		9
#define SHELL_S_START_Y		9
#define SHELL_L_START_X     9
#define SHELL_L_START_Y		52

#define MINEAREA_FRAME_X	12
#define MINEAREA_FRAME_Y	55


#define MINE_WIDTH			16
#define	MINE_HEIGHT			16
#define MINE_AREA_LEFT		12
#define MINE_AREA_TOP		55

//�������Ѷȼ��������
#define PRIMARY_XNUM		9		//����
#define PRIMARY_YNUM		9
#define PRIMARY_MINENUM		10

#define SECONDRY_XNUM		16		//�м�
#define SECONDRY_YNUM		16
#define SECONDRY_MINENUM	40

#define ADVANCE_XNUM		30		//�߼�
#define ADVANCE_YNUM		16
#define ADVANCE_MINENUM		99

//�������׷����״̬
#define STATE_NORMAL        0
#define STATE_FLAG			1
#define STATE_DICEY			2
#define STATE_BLAST			3
#define STATE_ERROR			4
#define STATE_MINE			5
#define STATE_DICEY_DOWN	6
#define STATE_NUM8			7
#define STATE_NUM7			8
#define STATE_NUM6			9
#define STATE_NUM5			10
#define STATE_NUM4			11
#define STATE_NUM3			12
#define STATE_NUM2			13
#define STATE_NUM1			14
#define STATE_EMPTY			15

#define ATTRIB_EMPTY		0
#define ATTRIB_MINE			1

//������Ĭ������:
#define DEFAULT_COLORFUL    1
#define DEFAULT_SOUNDFUL	0
#define DEFAULT_LEVEL		0

//��������Ϸ�Ѷ�:
#define LEVEL_PRIMARY       0
#define LEVEL_SECONDARY		1
#define LEVEL_ADVANCE		2
#define LEVEL_CUSTOM		3

//��������Ϸ״̬:
#define GS_WAIT				0
#define GS_RUN				1
#define GS_DEAD				2
#define GS_VICTORY			3

//�����ǰ�ť״̬:
#define BUTTON_DOWN			0
#define BUTTON_VICTORY		1
#define BUTTON_DEAD			2
#define BUTTON_CLICK		3
#define BUTTON_NORMAL		4


#endif
