import collections
import csv
import datetime
import decimal
from decimal import Decimal
import re

class Transaction(object):
  def __init__(self, 
      id, 
      amount,
      description,
      category):
    self.id = id
    self.date = datetime.datetime.strptime(self.id[:8], "%Y%m%d")
    self.amount = amount
    self.description = description
    self.category = category
  def __repr__(self):
    return "<%s,%s,%s,%s,%s>" % (
        self.id,
        self.date,
        self.amount,
        self.description,
        self.category)

class StoreMapper:
  def __init__(self):
    self.map = []
  def AddMapping(self, regexp, name):
    self.map.append((regexp, name))
  def GetStoreName(self, descr):
    for regexp, name in self.map:
      if re.match(regexp, descr):
        return name
    return descr

def ReadFile(fname):
  with open(fname, "rb") as f:
    reader = csv.DictReader(f)
    for row in reader:
      transaction = Transaction(
          row["Transaction ID"],
          Decimal(row["Amount"]),
          row["Description"],
          row["Transaction Category"])
      yield transaction
def AdjustCCDescription(orig_descr):
  if orig_descr.startswith("Ext Credit Card Debit "):
    orig_descr = orig_descr[22:]
  elif orig_descr.startswith("Ext Credit Card Credit "):
    orig_descr = orig_descr[23:]
  return orig_descr[:25].strip()

def main():
  stores = StoreMapper()
  stores.AddMapping(r"^ALASKA", "Alaska Airlines")
  stores.AddMapping(r"^AMAZON", "Amazon")
  stores.AddMapping(r"^Amazon", "Amazon")
  stores.AddMapping(r"^APL\*", "Apple")
  stores.AddMapping(r"^APPLE", "Apple")
  stores.AddMapping(r"^BIG 5 SPORTING GOODS", "Big 5")
  stores.AddMapping(r"^BIG WHITE", "BIGWHITE")
  stores.AddMapping(r"^BIGWHITE", "BIGWHITE")
  stores.AddMapping(r"^CHEVRON ", "Chevron")
  stores.AddMapping(r"^CHEVRON ", "Chevron")
  stores.AddMapping(r"^CHURRO - ", "Disneyland")
  stores.AddMapping(r"^DANCEWORKS ", "DANCEWORKS")
  stores.AddMapping(r".*DISNEY", "Disneyland")
  stores.AddMapping(r"^EXPEDIA", "Expedia")
  stores.AddMapping(r".*MEYER", "Fred Meyer")
  stores.AddMapping(r"^GOOD2GO", "GOOD2GO")
  stores.AddMapping(r"^HOMEDEPOT", "Home Depot")
  stores.AddMapping(r"^MACYS", "MACYS")
  stores.AddMapping(r"^MACYS", "MACYS")
  stores.AddMapping(r"^MARSHALLS", "MARSHALLS")
  stores.AddMapping(r"^MICHAELS", "MICHAELS")
  stores.AddMapping(r"^OLYMPUS SPA", "OLYMPUS SPA")
  stores.AddMapping(r"^PAPA MURPHY", "PAPA MURPHY'S")
  stores.AddMapping(r"^PAYPAL", "PAYPAL")
  stores.AddMapping(r"^QFC ", "QFC")
  stores.AddMapping(r"^ROSS STORES ", "Ross")
  stores.AddMapping(r"^SAFEWAY", "SAFEWAY")
  stores.AddMapping(r"^SPLASH MOUNTAIN", "Disneyland")
  stores.AddMapping(r"^TARGET", "Target")
  stores.AddMapping(r"^TRADER JOE'S ", "Trader Joes")
  stores.AddMapping(r"^TM\s\*", "Concert tickets")
  stores.AddMapping(r"^.*MOBILE", "T-Mobile")
  stores.AddMapping(r"^THE HOME DEPOT", "Home Depot")
  stores.AddMapping(r"^UBER", "Uber")
  stores.AddMapping(r"^WSFERRIES-", "Ferries")
  stores.AddMapping(r"^WSF GALLEY", "WSF GALLEY")
  stores.AddMapping(r".*- DL", "Disneyland")
  
  
  

  # checking = ReadFile("checking.csv")
  # checking = ReadFile("savings.csv")
  cc = list(ReadFile("credit-card.csv"))
  for r in cc:
    r.description = AdjustCCDescription(r.description)

  total_by_store = collections.defaultdict(Decimal)
  for r in cc:
    store = stores.GetStoreName(r.description)
    total_by_store[store] += r.amount

  # stores = sorted(list(total_by_store.keys()))
  # for s in stores:
  #   print(s)
  # print(len(stores))

  totals = sorted(total_by_store.iteritems(), key=lambda x: x[1])
  for t in totals:
    print("%s,%s" % (t[0], t[1]))

if __name__ == '__main__':
  main()

