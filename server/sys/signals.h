class SignalHandler {
  private:
    int mask, checkpoint;
  public :
    SignalHandler();
    int startup();
    void signalIgnore(void);
    void deadlockCheck(void);
    void signalShutdown(void);
    void sleep();
    void wake();
    void resetCheckpoint() {
      checkpoint = 0;
    }
};

extern SignalHandler *gSigHandler;

