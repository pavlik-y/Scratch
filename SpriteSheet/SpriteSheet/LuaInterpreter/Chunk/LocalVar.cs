using System;
using System.Collections.Generic;
using System.Text;

namespace Language.Lua
{
    public partial class LocalVar : Statement
    {
        public override LuaValue Execute(LuaTable enviroment, out bool isBreak)
        {
//            LuaValue[] values = this.ExprList.ConvertAll(expr => expr.Evaluate(enviroment)).ToArray();
            List<LuaValue> values = new List<LuaValue>();
            foreach (var expr in this.ExprList)
                values.Add(expr.Evaluate(enviroment));

            LuaValue[] neatValues = LuaMultiValue.UnWrapLuaValues(values.ToArray());

            for (int i = 0; i < Math.Min(this.NameList.Count, neatValues.Length); i++)
            {
                enviroment.RawSetValue(this.NameList[i], neatValues[i]);
            }

            if (neatValues.Length < this.NameList.Count)
            {
                for (int i = neatValues.Length; i < this.NameList.Count - neatValues.Length; i++)
                {
                    enviroment.RawSetValue(this.NameList[i], LuaNil.Nil);
                }
            }

            isBreak = false;
            return null;
        }
    }
}
