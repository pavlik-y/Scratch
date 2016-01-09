using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

using System.Diagnostics;

using System.Runtime.Serialization;
using Language.Lua;

namespace SpriteSheet
{
    class Env
    {
        public double TimeMs;
        public LuaValue GetTimeMs(LuaValue[] args)
        {
 //           Debug.WriteLine("GetTime");
            return new LuaNumber(TimeMs);
        }
    }

    class Item
    {
        SpriteSheet m_ss;
        Texture2D m_img;
        int m_spriteIndex;

        public Item(Texture2D img, SpriteSheet ss)
        {
            m_ss = ss;
            m_img = img;
        }

        public Texture2D GetImage()
        {
            return m_img;
        }

        public SpriteInfo GetSpriteInfo(TimeSpan gameTime)
        {
            return m_ss.GetSpriteById(m_spriteIndex);
        }

        public LuaValue SetSprite_FromScript(LuaValue[] args)
        {
//            Debug.WriteLine("{0}", args[0].Value);
            m_spriteIndex = (int)(args[0] as LuaNumber).Number;
            return null;
        }
    }

    [DataContract]
    public class SpriteInfo
    {
        public SpriteInfo(Rectangle r, Vector2 offset)
        {
            this.Rectangle = r;
            Offset = offset;
        }

        [DataMember]
        public Rectangle Rectangle;
        [DataMember]
        public Vector2 Offset;
    }

    [DataContract]
    public class SpriteSheet
    {

        [DataMember]
        public List<SpriteInfo> Sprites = null;

        public SpriteSheet()
        {
            Sprites = new List<SpriteInfo>();
        }

        public void AddSprite(Rectangle r, Vector2 offset)
        {
            Sprites.Add(new SpriteInfo(r, offset));
        }

        public int GetSpriteCount()
        {
            return Sprites.Count;
        }

        public SpriteInfo GetSpriteById(int spriteId)
        {
            return Sprites[spriteId];
        }
    }
}
