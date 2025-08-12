#pragma once



namespace imimr
{
	enum ImiGestureType
	{
		IMI_GESTURE_NONE,

		IMI_GESTURE_LEFT_HAND_WAVE,							// 1 ���ֻӶ�
		IMI_GESTURE_RIGHT_HAND_WAVE,						// 2 ���ֻӶ�

		IMI_GESTURE_LEFT_HAND_SWIPE_LEFT,					// 3 ���������
		IMI_GESTURE_LEFT_HAND_SWIPE_RIGHT,					// 4 �������һ�
		IMI_GESTURE_RIGHT_HAND_SWIPE_LEFT,					// 5 ���������
		IMI_GESTURE_RIGHT_HAND_SWIPE_RIGHT,					// 6 �������һ�

		IMI_GESTURE_LEFT_HAND_PUSH,							// 7 ������ǰ��
		IMI_GESTURE_RIGHT_HAND_PUSH,						// 8 ������ǰ��
		IMI_GESTURE_LEFT_HAND_HOOKUP,						// 9 �����Ϲ�
		IMI_GESTURE_RIGHT_HAND_HOOKUP,						// 10 �����Ϲ�

		IMI_GESTURE_LEAN_FORWARD,							// 11 ����ǰ��(������ͷ�ڷŽǶ�Ӱ��)
		IMI_GESTURE_LEAN_BACKWARD,							// 12 �������(������ͷ�ڷŽǶ�Ӱ��)
		IMI_GESTURE_LEAN_LEFT,								// 13 ��������
		IMI_GESTURE_LEAN_RIGHT,								// 14 ��������

		IMI_GESTURE_JUMP,									// 15 ��Ծ����Ҫ���ټ�⵽ϥ�ǣ�
		IMI_GESTURE_SQUAT,									// 16 ����
		IMI_GESTURE_WING,									// 17 ˫��ͬʱ���»Ӷ�
		IMI_GESTURE_RUN,									// 18 ˫��ǰ��ڶ�

		IMI_GESTURE_ZOOM_OUT,								// 19 ��С
		IMI_GESTURE_ZOOM_IN,								// 20 �Ŵ�

		IMI_GESTURE_LEFT_HAND_OVER_HEAD,					// 21 ���־ٹ�ͷ�������ַ���
		IMI_GESTURE_RIGHT_HAND_OVER_HEAD,					// 22 ���־ٹ�ͷ�������ַ���
		IMI_GESTURE_BOTH_HANDS_OVER_HEAD,					// 23 ˫�־ٹ�ͷ��
		IMI_GESTURE_BOTH_HANDS_BACK,						// 24 ˫�ַ��������

		IMI_GESTURE_MAIN_PAGE_BOTTOM_LEFT,							// 25 ��ж�������ҳ����
		IMI_GESTURE_MAIN_PAGE_BOTTOM_RIGHT,						// 26 ��ж�������ҳ����
		IMI_GESTURE_LEFT_ARM_FORWARD,						// 27 ���ǰ��
		IMI_GESTURE_RIGHT_ARM_FORWARD,						// 28 �ұ�ǰ��
		IMI_GESTURE_BOTH_ARMS_FORWARD,						// 29 ˫��ǰ��

		IMI_GESTURE_LEFT_ARM_LATERAL_RAISE,					// 30 ��۲�ƽ�٣��ұ�����£�
		IMI_GESTURE_RIGHT_ARM_LATERAL_RAISE,				// 31 �ұ۲�ƽ�٣��������£�
		IMI_GESTURE_BOTH_ARMS_LATERAL_RAISE,				// 32 ˫�ֲ�ƽ��
		IMI_GESTURE_RIGHT_ARM_LATERAL_RAISE_LEFT_ARM_UP,	// 33 �ұ۲�ƽ�٣�����Ͼ�
		IMI_GESTURE_RIGHT_ARM_UP_LEFT_ARM_LATERAL_RAISE,	// 34 ��۲�ƽ�٣��ұ��Ͼ�

		IMI_GESTURE_LEFT_KNEE_KICK,							// 35 ��ϥ��
		IMI_GESTURE_RIGHT_KNEE_KICK,						// 36 ��ϥ��
		IMI_GESTURE_LEFT_KNEE_LIFT,							// 37 ��ϥ̧�𣨾�̬��
		IMI_GESTURE_RIGHT_KNEE_LIFT,						// 38 ��ϥ̧�𣨾�̬��

		IMI_GESTURE_LEFT_FOOT_KICK,							// 39 ����ߣ���Ҫ��⵽��ţ�
		IMI_GESTURE_RIGHT_FOOT_KICK,						// 40 �ҽ��ߣ���Ҫ��⵽�ҽţ�

		IMI_GESTURE_BODY_MOVE_FORWARD,						// 41 ������ǰ��
		IMI_GESTURE_BODY_MOVE_BACKWARD,						// 42 ����������
		IMI_GESTURE_BODY_MOVE_LEFT,							// 43 ����������
		IMI_GESTURE_BODY_MOVE_RIGHT,						// 44 ����������

		IMI_GESTURE_TENNIS_SWIPE_LEFT,						// 45 ������Ϸ�����
		IMI_GESTURE_TENNIS_SWIPE_RIGHT,						// 46 ������Ϸ���һ�
		IMI_GESTURE_APPLAUSE,								// 47 ���ƣ�˫�ַֿ����ϲ���
		IMI_GESTURE_BOWLING_THROW,							// 48 ����������
		IMI_GESTURE_DEFENSE,								// 49 ����(˫�ַ�����ǰ)
		IMI_GESTURE_DRIVE,									// 50 ����
		IMI_GESTURE_RIGHT_HAND_THROW_DART,					// 51 �����ӷ���
		IMI_GESTURE_BASEBALL_HITTING,						// 52 �������

		IMI_GESTURE_LEFT_FOOT_STOMP,						// 53 �����̤����Ҫ��⵽��ţ�
		IMI_GESTURE_RIGHT_FOOT_STOMP,						// 54 �ҽ���̤����Ҫ��⵽�ҽţ�
		IMI_GESTURE_LEFT_FOOT_SIDE_KICK,					// 55 ��Ų��ߣ���Ҫ��⵽��ţ�
		IMI_GESTURE_RIGHT_FOOT_SIDE_KICK,					// 56 �ҽŲ��ߣ���Ҫ��⵽�ҽţ�
		IMI_GESTURE_SHAKE_FISTS,					        // 57 ������ȭ(�����Ⱦٹ��Ҽ磬�ٴ������)	
		IMI_GESTURE_GAME_SQUAT,                             // 58 ��Ϸ������߿ա��������� 
		IMI_GESTURE_MAIN_PAGE_TOP_LEFT,                    // 59 ��ж�������ҳ����
		IMI_GESTURE_MAIN_PAGE_TOP_RIGHT,					// 60 ��ж�������ҳ����
		IMI_GESTURE_LEFT_ARM_135_TO_90_DEGREE,              // 61 ��۴�135�Ȼ���90�ȣ��������������ƽ�У�,��̬����
		IMI_GESTURE_RIGHT_ARM_135_TO_90_DEGREE,				// 62 �ұ۴�135�Ȼ���90�ȣ������ұ�������ƽ�У�,��̬����
		IMI_GESTURE_LEFT_ARM_45_TO_90_DEGREE,              //  63 ��۴�45�Ȼ���90�ȣ��������������ƽ�У�,��̬����
		IMI_GESTURE_RIGHT_ARM_45_TO_90_DEGREE,				// 64 �ұ۴�45�Ȼ���90�ȣ������ұ�������ƽ�У�,��̬����
		IMI_GESTURE_MAIN_PAGE_CENTER_LEFT,                     // 65 ��ж�������ҳ���� 
		IMI_GESTURE_MAIN_PAGE_CENTER_RIGHT,                    // 66 ��ж�������ҳ����
		//IMI_GESTURE_LEFT_ARM_90_DEGREE,                        // 67 ��۲�ƽ��
		//IMI_GESTURE_RIGHT_ARM_90_DEGREE,                       // 68 �ұ۲�ƽ��
		IMI_GESTURE_COUNT
	};

	/** Gesture data */
	struct ImiGestureGameSquatData
	{
		enum GameSquatStatus {
			UNKNOWN = 0,
			START,
			DOWN,
			UP,
			KEEP,
			END
		};

		ImiGestureGameSquatData() :shoulderOffsetRate(0.f), squatStatus(UNKNOWN), kneeOpenRate(0.f) {}

		GameSquatStatus squatStatus;
		float               shoulderOffsetRate;
		float               kneeOpenRate;
	};


} // namespace imimr
