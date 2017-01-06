class DirectoryDTC:
  def IsReady(self):
    pass
  def Stop(self):
    # Disconnect
    # Unregister
    # StopSyncing
    # Stop models
    pass
  def StateTransition(self):
    if !self.IsReady():
      # UNREADY
      self.Stop()
      return
    if self.ShouldBeEncrypted() and self.IsPreferred() and pss.PassphraseRequired():
      # CRYPTO
      self.DiscardLocalChanges() # unapply
      self.Stop()
      return
    if self.IsDatatypeError() or self.UnrecoverableError():
      # FATAL
      self.Journal() #  Purge+journal
      self.Stop()
      return
    if !self.IsPrefered():
      # DISABLED
      self.Purge()
      self.Stop()
      return
    if self.IsPersistenceError() and self.IsCurrentlyConfigured():
      self.DiscardLocalChanges()




