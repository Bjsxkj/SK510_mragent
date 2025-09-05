#pragma once

#include "ImiMrApi.h"
#include "ImiGestureType.h"

#error This dile is deprecated

namespace imimr {

    REF_POINTER(ImiActionSensor)
    REF_POINTER(ImiActionFrame)

    

    /// @brief 骨架动作
    struct ImiActionItem {
		imimr::ImiGestureType type;
		//1������ɣ�2����������
		int state;
		//������ɵĽ���
		float progress;
		//�����÷� ��δ����
		float score;

		imimr::ImiGestureGameSquatData gameSquatData;

    };

    /// @brief 骨架动作数据
    struct ImiActionData {
        int userId;     ///< 用户Id,与骨架id对应.
        std::vector<ImiActionItem> actions; ///< 动作列表.
    };

    /// @brief 动作识别传感器，与ImiUserTrackSensor关联.
    class IImiActionSensor : public IImiBaseSensor {
        ImiUserTrackSensor userTrack;
        
    public:
        IImiActionSensor(ImiUserTrackSensor userTrack);
        virtual ~IImiActionSensor();

        static ImiActionSensor asMe(const ImiBaseSensor& p){ return p && p->getSensorType()==ImiSensorType::ACTION? std::dynamic_pointer_cast<IImiActionSensor>(p) : ImiActionSensor(); }

        // virtual ImiBaseFrame readFrame(int timeout=100) override; //{ return ImiBaseFrame(new IImiActionFrame(0)); }
        ImiBaseFrame readFrame(bool isCal, int timeout=100); //{ return ImiBaseFrame(new IImiActionFrame(0)); }

        virtual int start(int startflags = 0) override;

        virtual int stop() override;

        int addGesture(imimr::ImiGestureType gestureType);
		int removeGesture(imimr::ImiGestureType gestureType);
		int removeAllGestures();
        std::string getVersionOfGestureLib();

        static std::vector<ImiActionData> getActionData(DataBuffer foreground, const imimr::ImiBodySkeleton* pBodySkeleton);

    };

    /// @brief 动作识别数据帧
    class IImiActionFrame: public IImiBaseFrame {
    private:
        std::vector<ImiActionData> _actions;

    protected:
        ImiUserTrackFrame usertrackFrame;

    public:
        IImiActionFrame();
        virtual ~IImiActionFrame();

        void release();

        static ImiActionFrame asMe(const ImiBaseFrame& p){ return p && p->getSensorType()==ImiSensorType::ACTION? std::dynamic_pointer_cast<IImiActionFrame>(p) : ImiActionFrame(); }

        /// @brief 获取相关联的数据帧.
        /// @return 
        ImiUserTrackFrame getAccompanyFrame() { return usertrackFrame; }

        /// @brief 获取动作列表.
        /// @return 
        std::string getActionsData();
        
        friend class IImiActionSensor;
    
    };


}