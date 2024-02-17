using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Patterns_lab1_Strategy
{
    class TwoDigitAlgo: IAlg
    {
        public string Check(int val)
        {
            int right = (val / 10);
            int left = right / 10;
            return (left == 0 && right != 0) ? "Да, двузначное" : "Нет, не двузначное";
        }
    }
}
