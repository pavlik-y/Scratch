using System;
using System.Collections.Generic;
using System.Text;

namespace Language.Lua
{
    public class Tuple2<T1, T2>
    {
        public Tuple2(T1 i1, T2 i2)
        {
            Item1 = i1;
            Item2 = i2;
        }

        public T1 Item1;
        public T2 Item2;
    }

    public class Tuple3<T1, T2, T3>
    {
        public Tuple3(T1 i1, T2 i2, T3 i3)
        {
            Item1 = i1;
            Item2 = i2;
            Item3 = i3;
        }

        public T1 Item1;
        public T2 Item2;
        public T3 Item3;
    }
}