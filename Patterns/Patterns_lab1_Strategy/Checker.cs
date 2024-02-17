using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Patterns_lab1_Strategy
{
    class Checker
    {
        IAlg alg;

        public Checker(IAlg algo)
        {
            alg = algo;
        }

        public string Check(int val)
        {
            return alg.Check(val);
        }
    }
}
