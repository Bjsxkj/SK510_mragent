package test;


import com.sximi.mr.*;
import com.sximi.mrcore.*;
import java.nio.ByteBuffer;

public class Test1 {

    public static interface OnLog{
        void onLogPrint(String str);
    }

    public static class MyLog implements OnLog{
        @Override
        public void onLogPrint(String str){
            System.out.println(str);
        }
    }

    OnLog onLog;

    //AppContext app;

    IImiBaseDevice dev;

    public Test1(OnLog onLog){
        //this.app = app;
        this.onLog = onLog;
    }

//    public Test1(AppContext app, OnLog onLog){
//        this.app = app;
//        this.onLog = onLog;
//    }

    public  ImiDeviceAttribute[] dev_list;

    public boolean onInit(){
        //初始化SDK时调用
        ImiContext ctx = ImiContext.getContext();
        //
        dev_list = ctx.getDeviceList().toArray();
        if(dev_list.length>0) {
            onLog.onLogPrint(String.format("dev_list=[%s...]", dev_list[0]));
            dev = ctx.createDevice(dev_list[0]);
            return dev!=null;
        }
        return false;
    }

    public void onDestroy(){
        if(dev!=null){
            dev.close();
        }
        ImiContext.destroy();
    }

//        static class MyApp extends com.sximi.mr.AppContext{
//            @Override
//            public ImiDeviceAttributeList getUsbList(int flags) {
//                ImiDeviceAttributeList r = new ImiDeviceAttributeList();
//                r.add(new ImiDeviceAttribute());
//                return r;
//            }
//            @Override
//            public int getUsbManager() {
//                return 0;
//            }
//            @Override
//            public int openUsbHandle(ImiDeviceAttribute devAttribute){
//                return -1;
//            }
//        }

        public void testColor(IImiBaseSensor sensor){
            IImiColorSensor color = IImiColorSensor.asMe(sensor);
            if(color!=null){
                onLog.onLogPrint(String.format("color={0}", color));

                color.start(0);

                for(int i=0;i<10;i++){
                    IImiBaseFrame frame = color.readFrame(100);
                    IImiColorFrame color_frame = IImiColorFrame.asMe(frame);
                    System.out.println(String.format("frame={0} , {1}", color_frame, frame));
                }

                color.stop();

                color.release();
            }
        }

        public static void main(String[] args) {
            System.out.println(String.format("hello"));

            ImiContext.create(null);

            Test1 test = new Test1(new MyLog());

            test.testAll();

        }

        public void testAll(){

            //AppContext app = new AppContext();

            MessageCore core = new MessageCore();

            core.initialize("./config.ini");

            core.start();


            ImiContext ctx = ImiContext.getContext();

            ImiDeviceAttribute[] dev_list = ctx.getDeviceList().toArray();

            
            if(dev_list.length>0){
                onLog.onLogPrint(String.format("dev_list=[%s...]", dev_list[0]));

                IImiBaseDevice dev = ctx.createDevice(dev_list[0]);

                
                if(dev!=null){

                    onLog.onLogPrint(String.format("device={0}", dev!=null?dev.toString():"null"));

                    onLog.onLogPrint(String.format("device ver={0}", dev.getVersions()));


                    dev.open((int)ImiSensorType.COLOR | (int)ImiSensorType.USERTRACK);

                    IImiBaseSensor sensor = dev.getSensor(ImiSensorType.USERTRACK);

                    if(sensor!=null){

                        sensor.start(0);

                        for(int i=0; i<1000; i++){
                            IImiBaseFrame fr = sensor.readFrame();
                            IImiUserTrackFrame f = IImiUserTrackFrame.asMe(fr);

                            if(f!=null){
                                ImiBodySkeleton ske = f.getBodySkeleton();
                                core.syncSkeletonData(ske);

                                f.release();
                            }
                            fr = null;
                        }

                        //System.GC();

                        sensor.stop();
                    }

                    

                    dev.close();


                }





            }


            ImiContext.destroy();

            core.stop();

        }
    }