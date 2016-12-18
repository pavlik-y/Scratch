enabled_types
types_being_configured
catch_up_in_progress
unready_error
last_requested_types
crypto_error
data_type_error
unrecoverable_error
persistence_error
initial_sync_done_types



disabled_types = []
unapply_types = []
fatal_types = []
types_to_add = []
ready_types = []
for t in types:
  if t in unready_error and t in last_requested_types: // UNREADY
    disabled_types += t
    continue
  if t in crypto_error: // CRYPTO
    disabled_types += t
    unapply_types += t
    continue
  if t in data_type_error or t in unrecoverable_error: // FATAL
    disabled_types += t
    fatal_types += t
    continue
  if t in (UserTypes() + ControlTypes()) and t not in enabled_types: // DISABLED
    disabled_types += t
    continue
  if (t in persistence_error and t in types_being_configured)
      or (catch_up_in_progress and t in enabled_types): // CONFIGURE_CLEAN
    types_to_add += t
    if t != PROXY_TABS:
      types_to_download += t
    types_to_purge += t
    unapply_types += t
    continue
  if t in enabled_types and t in types_being_configured: // CONFIGURE_ACTIVE
    if t not in initial_sync_done_types:
      types_to_add += t
      if t != PROXY_TABS:
        types_to_download += t
    else:
      ready_types += t
    continue
  if t in enabled_types: // CONFIGURE_INACTIVE















disabled_types => Remove from routing info