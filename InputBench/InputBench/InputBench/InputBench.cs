using System;
using System.Diagnostics;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Xna.Framework.Media;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;

namespace InputBench
{
    public class InputBench : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager m_graphics;

        SpriteBatch m_spriteBatch;

        RenderTarget2D m_image;

        Texture2D m_brush;

        TimeSpan m_lastClear = TimeSpan.MinValue;
        
        LinkedList<string> m_log;

        TimeSpan m_timeDiff = TimeSpan.MinValue;

        bool m_hadTouch = false;

        public InputBench()
        {
            m_graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            TargetElapsedTime = TimeSpan.FromTicks(333333);

            TouchPanel.EnabledGestures = GestureType.Tap | GestureType.FreeDrag | GestureType.DragComplete | 
                GestureType.Pinch | GestureType.PinchComplete | GestureType.Hold;
            m_log = new LinkedList<string>();
            PhoneApplicationService.Current.Launching += new EventHandler<LaunchingEventArgs>(Current_Launching);
            PhoneApplicationService.Current.Closing += new EventHandler<ClosingEventArgs>(Current_Closing);
            PhoneApplicationService.Current.Activated += new EventHandler<ActivatedEventArgs>(Current_Activated);
            PhoneApplicationService.Current.Deactivated += new EventHandler<DeactivatedEventArgs>(Current_Deactivated);
            Debug.WriteLine("ctor");
        }

        void Current_Deactivated(object sender, DeactivatedEventArgs e)
        {
            Debug.WriteLine("Deactivated");
        }

        void Current_Activated(object sender, ActivatedEventArgs e)
        {
            Debug.WriteLine("Activated: {0}",e.IsApplicationInstancePreserved);
        }

        void Current_Closing(object sender, ClosingEventArgs e)
        {
            Debug.WriteLine("Closing");
        }

        void Current_Launching(object sender, LaunchingEventArgs e)
        {
            Debug.WriteLine("Launching");
        }

        protected override void Initialize()
        {
            Debug.WriteLine("Initialize");
            base.Initialize();
        }

        protected override void LoadContent()
        {
            Debug.WriteLine("LoadContent");
            m_brush = Content.Load<Texture2D>("SmallBrush");
            m_spriteBatch = new SpriteBatch(GraphicsDevice);
            m_image = new RenderTarget2D(
                GraphicsDevice,
                GraphicsDevice.PresentationParameters.BackBufferWidth,
                GraphicsDevice.PresentationParameters.BackBufferHeight,
                false, SurfaceFormat.Color,
                DepthFormat.None, 0,
                RenderTargetUsage.PreserveContents);
        }

        protected override void UnloadContent()
        {
            Debug.WriteLine("UnloadContent");
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            base.Update(gameTime);

            bool bHadInput = false;
            GraphicsDevice.SetRenderTarget(m_image);
            if (m_lastClear == TimeSpan.MinValue ||
                (gameTime.TotalGameTime - m_lastClear).TotalSeconds > 10)
            {
                m_lastClear = gameTime.TotalGameTime;
                GraphicsDevice.Clear(Color.Transparent);
            }
            m_spriteBatch.Begin();

            while (TouchPanel.IsGestureAvailable)
            {
                GestureSample gs = TouchPanel.ReadGesture();
                if (m_timeDiff == TimeSpan.MinValue)
                    m_timeDiff = gs.Timestamp - gameTime.TotalGameTime;

                Color color = Color.Black;
                switch (gs.GestureType)
                {
                    case GestureType.Tap:
//                        color = Color.Yellow;
                        break;
                    case GestureType.FreeDrag:
//                        color = Color.Orange;
                        break;
                    case GestureType.DragComplete:
                        color = Color.Red;
                        break;
                    case GestureType.Pinch:
                        color = Color.Purple;
                        break;
                    case GestureType.PinchComplete:
                        color = Color.Blue;
                        break;
                    case GestureType.Hold:
                        EmailComposeTask task = new EmailComposeTask();
                        task.To = "pavely@microsoft.com";
                        task.Subject = "Test";
                        task.Body = "HelloWorld!!!\nOtherLine";
                        task.Show();
                        break;
                }

                m_spriteBatch.Draw(m_brush, gs.Position, color);
                bHadInput = true;
                m_log.AddFirst(string.Format("{0}: type={1}, pos={2}, pos2={3}", gs.Timestamp - m_timeDiff, gs.GestureType, gs.Position, gs.Position2));
            }
            TouchCollection tc = TouchPanel.GetState();
            foreach (TouchLocation tl in tc)
            {
 //               if (!m_hadTouch)
                {
                    m_spriteBatch.Draw(m_brush, tl.Position + new Vector2(-20, -20), Color.Green);
//                    m_spriteBatch.Draw(m_brush, tl.Position, Color.Green);
                    m_log.AddFirst(string.Format("{0}: {1}", gameTime.TotalGameTime, tl));
                }
                bHadInput = true;
            }
            
            m_spriteBatch.End();
            GraphicsDevice.SetRenderTarget(null);
            if (bHadInput)
            {
                m_hadTouch = true;
            }
            else
            {
                if (m_hadTouch)
                {
                    m_log.AddFirst(string.Format("{0}: No touch", gameTime.TotalGameTime));
                    m_hadTouch = false;
                }
                for (int i = 0; i < 5 && m_log.Count > 0; i++)
                {
//                    Debug.WriteLine(m_log.Last.Value);
                    m_log.RemoveLast();
                }
            }
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Black);
            m_spriteBatch.Begin();
            m_spriteBatch.Draw(m_image, Vector2.Zero, Color.White);
            m_spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
