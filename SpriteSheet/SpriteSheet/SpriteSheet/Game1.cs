using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Xna.Framework.Media;
using System.IO;
using System.Diagnostics;

using Language.Lua;

namespace SpriteSheet
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D m_sunImg;
        SpriteSheet m_sunSS;
        Item m_sun;
        Env m_env;
        LuaTable m_globals;

        Language.Lua.Chunk m_script;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Extend battery life under lock.
            InactiveSleepTime = TimeSpan.FromSeconds(1);
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // TODO: use this.Content to load your game content here
            m_env = new Env();
            m_sunImg = Content.Load<Texture2D>("sun");
            string sunSpriteSer = @"
{Sprites:[
    {Offset:{X:0,Y:0},Rectangle:{Height:122,Width:158,X:0,Y:0}},
    {Offset:{X:4,Y:0},Rectangle:{Height:122,Width:135,X:158,Y:0}}
]}
";
            MemoryStream ms = new MemoryStream();
            StreamWriter sw = new StreamWriter(ms);
            sw.Write(sunSpriteSer);
            sw.Flush();
            ms.Flush();
            ms.Seek(0, SeekOrigin.Begin);
            System.Runtime.Serialization.Json.DataContractJsonSerializer serializer = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(SpriteSheet));
            m_sunSS = (SpriteSheet)serializer.ReadObject(ms);
            m_sun = new Item(m_sunImg, m_sunSS);
            string script = "a != b";
/*
            string script = @"
item.SetSprite((env.GetTimeMs() / 200) % 2);
item.SetSprite = 0;
";
*/
            Parser parser = new Parser();
            TextInput ti = new TextInput(script);
            bool succ;
            m_script = parser.ParseChunk(ti, out succ);
            if (!succ)
                Debug.WriteLine(parser.GetEorrorMessages());
            m_globals = new LuaTable();
            LuaTable methods = new LuaTable();
            methods.SetNameValue("__index", methods);
            methods.SetNameValue("__newindex", methods);
            methods.Register("SetSprite", m_sun.SetSprite_FromScript);
            m_globals.SetNameValue("item", new Language.Lua.LuaUserdata(m_sun, methods));

            methods = new LuaTable();
            methods.SetNameValue("__index", methods);
            methods.Register("GetTimeMs", m_env.GetTimeMs);
            m_globals.SetNameValue("env", new LuaUserdata(m_env, methods));

            m_env.TimeMs = 0;
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // TODO: Add your update logic here
            m_env.TimeMs = gameTime.TotalGameTime.TotalMilliseconds;
            bool isBreak;
            m_script.Execute(m_globals, out isBreak);
            Debug.Assert(isBreak == false);

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);
            spriteBatch.Begin();
//            spriteBatch.Draw(ss.Image, Vector2.Zero, ss.GetSpriteById(0), Color.White);
            SpriteInfo si = m_sun.GetSpriteInfo(gameTime.TotalGameTime);
            spriteBatch.Draw(m_sun.GetImage(), Vector2.Zero + si.Offset, si.Rectangle, Color.White);
            // TODO: Add your drawing code here
            spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
