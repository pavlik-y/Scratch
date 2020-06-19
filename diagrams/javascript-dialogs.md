- If i navigate to bar.com on the first alert then the message is "An embedded page on this page says"
  - IDS_JAVASCRIPT_MESSAGEBOX_TITLE_NONSTANDARD_URL_IFRAME
- If I just tap Ok, then the message is "This page says"
  - IDS_JAVASCRIPT_MESSAGEBOX_TITLE_NONSTANDARD_URL

- Going through about:blank dismisses all dialogs and continues navigation
- going through google.com only dismisses first dialog and blocks navigation.
- Location where these messages are generated: components/javascript_dialogs/app_modal_dialog_manager.cc
- Seems all dismissals go through AppModalDialogViewAndroid::~AppModalDialogViewAndroid
- TabModalDialogManagerDelegate::CreateNewDialog
- chrome/browser/ui/javascript_dialogs/javascript_tab_modal_dialog_manager_delegate_android.h





- Questions:
  - How android view is displayed and removed?
  - Trace native callstacks where the view is removed