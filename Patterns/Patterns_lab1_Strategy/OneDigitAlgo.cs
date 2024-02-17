using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Patterns_lab1_Strategy
{
    class OneDigitAlgo: IAlg
    {
        public string Check(int val)
        {
            int tmp = val / 10;
            return (tmp == 0) ? "Да, однозначное" : "Нет, не однозначное";
        }
    }
}
