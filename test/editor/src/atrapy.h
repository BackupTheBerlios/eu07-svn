class foo : public GL_realizer
{
public:
	foo ( GtkWidget* dr_area ) : GL_realizer(dr_area) {};
	virtual gboolean button ( GdkEventButton * ) { return false; };
	virtual gboolean motion ( GdkEventMotion * ) { return false; };
    virtual gboolean enter_notify ( GdkEventCrossing * ) { return false; };
    virtual gboolean leave_notify ( GdkEventCrossing * ) { return false; };
    virtual gboolean scroll ( GdkEvent * ) { return false; };
    
	virtual void resized ( )
	{
		begin_gl();
  glViewport (0, 0,
              get_area_alloc()->width, get_area_alloc()->height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (40.0, (float)get_area_alloc()->width/(float)get_area_alloc()->height, 0.1, 1000.0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gluLookAt (0.0, 0.0, 3.0,
             0.0, 0.0, 0.0,
             0.0, 1.0, 0.0);
		 end_gl();
	};
	virtual gboolean exposed ( ) 
	{ 
	    printf("foo exposed\n");
			begin_gl();
  glClearColor (0.0, 1.0, 1.0, 1.0);
  glClearDepth (1);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1,0,0);
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(0,1,0);
  glVertex3f(1,0,0);
  glVertex3f(-1,0,0);
  glVertex3f(0,-1,0);
  glEnd();
		 commit_gl();
		 end_gl();
		return true; 
	};
};

class Editor
{
    public:
       	typedef enum { 
            em_Select=0, em_Move, em_Rotate, em_CreatePoints,
            em_CreatePointFeatures, em_CreateLines, em_ConnectPoints,
            em_CreateRailLines, em_CreateTracks, em_CreateFence, em_Divide,
            em_Fill, em_CreateSignals, em_CreateCatenary, em_Import } EditorMode;

        void loadFromFile(const char *n) {};
        void saveToFile(const char *n) {};
        static Editor *instance()
        {
            static Editor ein;
            return &ein;
        };
        bool changeMode(EditorMode _mode) {};
        void freeNodes() {};
        void exportToDirectory(const char *) {};
        void toggleCurve() {};
        void applyFeatures() {};
        void freeSelectedNode() {};
};

