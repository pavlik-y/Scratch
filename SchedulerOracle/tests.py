import scheduler

def TestPoll():
  poll_tracker = scheduler.PollTracker()
  poll_tracker.SetTime(20)
  assert(poll_tracker.GetNextSyncTime() == (scheduler.long_ago, scheduler.long_ago + scheduler.never))
  poll_tracker.SetPollInterval(100)
  poll_tracker.StartedSyncCycle()
  assert(poll_tracker.GetNextSyncTime() == (scheduler.long_ago, 120))
  poll_tracker.SetTime(30)
  assert(poll_tracker.GetNextSyncTime() == (scheduler.long_ago, 120))
  poll_tracker.StartedSyncCycle()
  assert(poll_tracker.GetNextSyncTime() == (scheduler.long_ago, 130))


def TestThrottleTracker():
  throttle_tracker = scheduler.ThrottleTracker()
  throttle_tracker.SetTime(10)
  throttle_tracker.FinishedSyncCycle(True)
  assert(throttle_tracker.GetNextSyncTime()[1] == scheduler.never)
  assert(throttle_tracker.GetNextSyncTime()[0] <= 10)
  throttle_tracker.ReceivedThrottledInterval(100)
  throttle_tracker.FinishedSyncCycle(False)
  assert(throttle_tracker.GetNextSyncTime()[0] == 110)
  throttle_tracker.SetTime(120)
  throttle_tracker.FinishedSyncCycle(False)
  assert(throttle_tracker.GetNextSyncTime()[0] == 220)
  throttle_tracker.SetTime(230)
  throttle_tracker.FinishedSyncCycle(True)
  assert(throttle_tracker.GetNextSyncTime()[0] <= 230)




def TestRetryTracker():
  retry_tracker = scheduler.RetryTracker()
  retry_tracker.SetTime(20)
  assert(retry_tracker.GetNextSyncTime() == (scheduler.long_ago, scheduler.never))
  retry_tracker.ReceivedRetryGUInterval(20)
  assert(retry_tracker.GetNextSyncTime() == (scheduler.long_ago, 40))
  retry_tracker.SetTime(30)
  assert(retry_tracker.GetNextSyncTime() == (scheduler.long_ago, 40))
  retry_tracker.ReceivedRetryGUInterval(20)
  assert(retry_tracker.GetNextSyncTime() == (scheduler.long_ago, 50))
  assert(retry_tracker.ShouldRunRetryGU() == False)
  retry_tracker.SetTime(50)
  assert(retry_tracker.ShouldRunRetryGU() == True)
  retry_tracker.StartedRetryGU()
  retry_tracker.ReceivedRetryGUInterval(20)
  retry_tracker.SetTime(60)
  assert(retry_tracker.GetNextSyncTime() == (scheduler.long_ago, 60))
  assert(retry_tracker.ShouldRunRetryGU() == True)
  retry_tracker.RetryGUSucceeded()
  assert(retry_tracker.GetNextSyncTime() == (scheduler.long_ago, 70))
  assert(retry_tracker.ShouldRunRetryGU() == False)

def TestEBRTracker():
  ebr = scheduler.EBRTracker()
  ebr.SetTime(10)



def main():
  TestPoll()
  TestThrottleTracker()
  TestRetryTracker()
  TestEBRTracker()
  print("Done")

if __name__ == '__main__':
  main()

