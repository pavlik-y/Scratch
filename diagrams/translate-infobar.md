## I am removing SnackbarManager reference from InfoBar

- InfoBar.getSnackarManager is used by TranslateCompactInfoBar

## Trace call to TranslateCompactInfoBar

- TranslateCompactInfoBar#create
- TranslateCompactInfoBar(native)::CreateRenderInfoBar

- TranslateCompactInfoBar(native)::ctor
- ChromeTranslateClient::CreateInfoBar (in translate_compact_infobar.cc)
  - Overrides TranslateClient::CreateInfoBar
- TranslateInfoBarDelegate::Create (translate_infobar_delegate.cc)
- ChromeTranslateClient::ShowTranslateUI
  - Overrides TranslateClient::ShowTranslateUI
- TranslateManager -> ShowTranslateUI


- ChromeActivity
- TranslateBridge#translateTabWhenReady
- JNI_TranslateBridge_ManualTranslateWhenReady
  - Has access to current WebContents
- ChromeTranslateClient::ManualTranslateWhenReady
- TranslateManager::InitiateManualTranslation



- ChromeTranslateClient is created in tab_helpers.cc
  - ChromeTranslateClient::CreateForWebContents(web_contents);
ChromeTranslateClient::CreateInfoBar


## Steps
- Pass WebContents from ChromeTranslateClient to TranslateCompactInfoBar and stash it there.
  - What about WebContents lifetime?
    - Use WebContentsObserver
- Translate from WebContents to Tab(java) and pass the tab to TranslateCompactInfoBar_Create
- Translate from Tab to SnackbarManageable in TranslateCompactInfoBar#create and pass SnackbarManageable to ctor
- Get snackbar manager from stashed SnackbarManageable