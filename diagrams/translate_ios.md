chrome_ios_translate_client.mm: ChromeIOSTranslateClient::ShowTranslateUI

- TranslateInfoBarDelegate::Create()
- ChromeIOSTranslateClient::CreateInfoBar(delegate)
  - Here codepath switches between infobars and messages.
  - Creates TranslateInfoBarCoordinator (delegate)
  - Creates InfoBarIOS (coordinator, delegate)

- TranslateInfobarCoordinator is in infobar_translate_coordinator.h file


- InfobarContainerCoordinator::addInfoBarWithDelegate
  - InfoBarCoordinator.start()

- TranslateInfobarCoordinator::start()
  - Creates InfobarTranslateMediator