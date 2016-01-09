long_ago = -10000
never = 10000
infinite = 10000

class BaseTracker:
  def __init__(self):
    self.time = 0
  def SetTime(self, time):
    self.time = time

class RetryTracker(BaseTracker):
  def __init__(self):
    BaseTracker.__init__(self)
    self.next_retry_gu_time = never
    self.last_started_retry_gu = long_ago
    self.last_successful_retry_gu = long_ago
  def ReceivedRetryGUInterval(self, interval):
    self.next_retry_gu_time = self.time + interval
  def StartedRetryGU(self):
    self.last_started_retry_gu = self.time
  def RetryGUSucceeded(self):
    self.last_successful_retry_gu = self.time
  def GetNextSyncTime(self):
    assert(self.time >= self.last_started_retry_gu)
    if (self.last_started_retry_gu > self.last_successful_retry_gu):
      return (long_ago, self.last_started_retry_gu)
    return (long_ago, self.next_retry_gu_time)
  def ShouldRunRetryGU(self):
    return self.GetNextSyncTime()[1] <= self.time

class PollTracker(BaseTracker):
  def __init__(self):
    BaseTracker.__init__(self)
    self.poll_interval = infinite
    self.last_sync_attempt = long_ago
  def SetPollInterval(self, interval):
    self.poll_interval = interval
  def StartedSyncCycle(self):
    self.last_sync_attempt = self.time
  def GetNextSyncTime(self):
    return (long_ago, self.last_sync_attempt + self.poll_interval)

class ThrottleTracker(BaseTracker):
  def __init__(self):
    BaseTracker.__init__(self)
    self.throttled_interval = 0
    self.throttled_until = long_ago
  def ReceivedThrottledInterval(self, interval):
    self.throttled_interval = interval
  def FinishedSyncCycle(self, success):
    if success:
      self.throttled_interval = 0
    self.throttled_until = self.time + self.throttled_interval
  def GetNextSyncTime(self):
    return (self.throttled_until, never)

class EBRTracker(BaseTracker):
  def __init__(self):
    BaseTracker.__init__(self)
    self.backoff_interval = 5
    self.failed_attempts_count = 0
//    self.last_attempt_time = long_ago
    self.backoff_time = long_ago
  def FinishedSyncCycle(self, success):
    if success:
      self.failed_attempts_count = 0
      self.backoff_time = self.time
    elif self.backoff_time < self.time:
      self.failed_attempts_count += 1
      self.backoff_time = self.time +
  def GetNextSyncTime(self):
    return (never, never)


class SchedulerOracle:
  def __init__(self):
    self.time = 0
    self.retry_tracker = RetryTracker()
    self.poll_tracker = PollTracker()
  def SetTime(self, time):
    self.time = time
    self.retry_tracker.SetTime(time)
    self.poll_tracker.SetTime(time)
  def SetPollInterval(self, interval):
    self.pollInterval = interval
  def ReceivedGlobalThrottled(self, delay):
    self.throttledUntil = self.time + delay
  def ReceivedRetryGU(self, interval):
    pass
  def ReportCycleAttempt(self):
    self.lastCycleAttempt = self.time
  def GetNextCycleTime(self):
    nextCycleTime = self.lastCycleAttempt + self.pollInterval
    if self.throttledUntil > nextCycleTime:
      nextCycleTime = self.throttledUntil
    return nextCycleTime
  def CycleShouldSetRetryFlag(self):
    return False

