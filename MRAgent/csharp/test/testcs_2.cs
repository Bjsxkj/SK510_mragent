using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using imi.mr.agent;

namespace test
{
   
    public class Test1
    {
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

            ImiForegroundData dt = new ImiForegroundData();
            System.Console.WriteLine("sn = {0} == {1}", dt, dt.getHandle());

            ImiBodySkeleton body = new ImiBodySkeleton(); // DataUtil.createImiBodySkeleton();

            System.Console.WriteLine("body = {0} == {1}", body, body.getHandle());

        }
    }

}