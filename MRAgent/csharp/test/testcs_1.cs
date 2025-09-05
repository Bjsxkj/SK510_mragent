
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using imi.mr.agent;



namespace test
{
    // public class MyCallback : IMyCallback {
    //     public override void onCall(int a, ABC abc){
    //         System.Console.WriteLine("call onCall in C#");
    //     }
    // }
   
    public class Test1
    {
        void TestColor(IImiBaseSensor sensor){
            if(sensor==null){
                return;
            }
            //类型转换
            IImiColorSensor color = IImiColorSensor.asMe(sensor);

            // 第五步：获取数据操作
            if(color!=null){
                System.Console.WriteLine("color={0}", color);
                // 开始输出数据流.
                color.start(0);

                for(int i=0;i<50;i++){
                    // 获取一帧数据.
                    IImiBaseFrame frame = color.readFrame(100);
                    if(frame!=null){
                        IImiColorFrame color_frame = IImiColorFrame.asMe(frame);
                        System.Console.WriteLine("frame={0} , {1}", color_frame, frame);
                        int w = color_frame.getWidth();
                        int h = color_frame.getHeight();
                        // 读取图像数据
                        byte[] data = new byte[w*h*3];
                        color_frame.getData(data, w*h*3);
                        // todo: 渲染显示图像

                        break;
                    }
                }

                System.Console.WriteLine("stop");

                // 停止输出并释放资源.
                color.stop();

                color.release();
            }
        }

        void TestDepth(IImiBaseSensor sensor){
            if(sensor==null){
                return;
            }
            //类型转换
            IImiDepthSensor ss = IImiDepthSensor.asMe(sensor);

            // 第五步：获取数据操作
            if(ss!=null){
                System.Console.WriteLine("sensor={0}", ss);
                // 开始输出数据流.
                ss.start(0);

                for(int i=0;i<50;i++){
                    // 获取一帧数据.
                    IImiBaseFrame frame = ss.readFrame(100);
                    if(frame!=null){
                        IImiDepthFrame d_frame = IImiDepthFrame.asMe(frame);
                        System.Console.WriteLine("frame={0} , {1}", d_frame, frame);
                        int w = d_frame.getWidth();
                        int h = d_frame.getHeight();
                        // 读取图像数据,深度图的原始和显示数据是不一样的
                        byte[] rawdata = new byte[w*h*2];
                        d_frame.getData(rawdata, w*h*2);
                        byte[] showdata = new byte[w*h*3];
                        d_frame.getRgbData(showdata, w*h*3);
                        // todo: 渲染显示图像

                        break;
                    }
                }

                System.Console.WriteLine("stop");

                // 停止输出并释放资源.
                ss.stop();

                ss.release();
            }
        }

        void ShowDevInfo(IImiBaseDevice dev){
            // todo: 
            // IStringProperty sn = new IStringProperty(32);
            // dev.getProperty((int)IImiBaseDevice.ImiDeviceProperties.IMI_PROPERTY_RESET_DEVICE, sn);
            // System.Console.WriteLine("sn = {0}", sn);
        }


        void TestAction(IImiActionSensor sensor){
            sensor.start();

            for(int i=0;i<50;i++){
                IImiActionFrame frame = IImiActionFrame.asMe(sensor.readFrame(100));
                if(frame!=null){
                    System.Console.WriteLine("fr = {0}", frame);
                }
            }

            System.Console.WriteLine("stop");
            sensor.stop();
            sensor.release();
        }

        void TestUserTrack(IImiUserTrackSensor sensor){
            sensor.start();

            for(int i=0;i<50;i++){
                IImiUserTrackFrame frame = IImiUserTrackFrame.asMe(sensor.readFrame(100));
                if(frame!=null){
                    System.Console.WriteLine("fr = {0}", frame);

                    ImiBodySkeleton ske = frame.getBodySkeleton();

                    System.Console.WriteLine("fr = {0}", ske);

                }
            }

            System.Console.WriteLine("stop");
            sensor.stop();
            sensor.release();
        }


        static void Main(string[] args){
            System.Console.WriteLine("hello");
            // 程序开始时创建上下文
            // U3D Android平台使用 ：ImiContext.create(new imi.mr.u3d.ImiU3DAppContext());
            // PC平台传null即可.
            ImiContext.create(null);

            Test1 test1 = new Test1();
            test1.RunTest();
            // 程序退出前销毁上下文
            // ImiContext.destroy();
        }

        void RunTest(){

            //第一步：获取ImiContext，ImiContext是全局单列的。
            ImiContext ctx = ImiContext.getContext();
            //第一步：连接的USB设备列表
            // ImiDeviceAttribute[] dev_list = ctx.getDeviceList().ToArray();

            bool ok = ctx.initialize("./mragent.toml");

            System.Console.WriteLine("OK={0}", ok);


            // System.Console.WriteLine("dev_list={0}", dev_list);
            
            if(ok){
                // System.Console.WriteLine("dev_list=[{0}]", dev_list);
                //第三步：创建设备，
                IImiBaseDevice dev = ctx.createDevice();

                
                
                if(dev!=null){

                    System.Console.WriteLine("device={0}", dev!=null?dev.ToString():"null");

                    System.Console.WriteLine("device ver={0}", dev.getVersions());

                    ShowDevInfo(dev);

                    //第四步：按需打开需要类型的Sensor
                    dev.open((int)ImiSensorType.USERTRACK);

                    //IImiColorSensor sensor = IImiColorSensor.asMe(dev.getSensor(ImiSensorType.COLOR));
                    IImiUserTrackSensor usertarack = IImiUserTrackSensor.asMe(dev.getSensor(ImiSensorType.USERTRACK));
                    //IImiActionSensor action = IImiActionSensor.asMe(dev.getSensor(ImiSensorType.ACTION));

                    System.Console.WriteLine("device ver={0}", usertarack);

                    // usertarack.start();

                    TestUserTrack(usertarack);

                    //TestDepth(depth);

                    dev.close();
                }

            }


            // ABC a = new ABC();
            // a.a = 15;

            // System.Console.WriteLine("a="+a);

            // byte[] bb = new byte[16];
            // api_cs.getArrayDataTo(bb, 16);
            // System.Console.WriteLine("bb="+bb[1]);

            // int[] ii = new int[4] {1,2,3,4};
            // int n = ii.Length;
            // api_cs.printIntArr(ii);

            // MyCallback mycall = new MyCallback();
            // api_cs.call(mycall, 20);

            
            

        }
    }
}