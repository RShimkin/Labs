using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;

namespace ConsoleApp1
{ 

    public class asy
    {
        public async Task<string> ShowMessage()
        {
            await Task.Delay(1000);
            return "Message!";
        }
    }

    class MyClass
    {
        public string Field { get; set; }
    }

    class BaseClass
    {
        public virtual void Method() { }
    }
    class ChildClass : BaseClass 
    { 
        public int Method() {
            return 5;
        }
    }

    abstract class Ba
    {
        public Ba()
        {
            Console.WriteLine("Ba");
        }
    }

    class Chi : Ba
    {
        public Chi()
        {
            Console.WriteLine("Chi");
        }
    }

    abstract class AbstractClass
    {
        public AbstractClass()
        {
            Console.WriteLine("create abstract");
        }

        public virtual string Name { get; set; } = "Abstract";
        public void PrintName()
        {
            Console.WriteLine(Name);
        }
    }

    class ConcreteClass: AbstractClass
    {
        public ConcreteClass()
        {
            Console.WriteLine("create concrete");
        }
        public override string Name { get; set; } = "Concrete class";
        public void PrintName()
        {
            base.PrintName();
        }
    }

    class Program
    {
        public static int globalvar = 32;

        static MyClass Creator()
        {
            var myClass = new MyClass
            {
                Field = "a"
            };
            try
            {
                throw new Exception();
            }
            catch
            {
                myClass.Field += "b";
                return myClass;
            }
            finally
            {
                myClass.Field += "c";
            }
        }
        private static int? myfunc(int x)
        {
            if (x < 100)
                return x;
            return null;
        }
        
        public static void Main(string[] args)
        {
            //Work.Task();
            //asy aaa = new asy();
            //Console.WriteLine(aaa.ShowMessage().GetAwaiter().GetResult());
            var myClass = Creator();
            Console.Write(myClass.Field);

            //Ba ba = new Chi();
            //Chi chi = new Chi();

        }
    }
}
