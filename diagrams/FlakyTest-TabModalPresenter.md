- TestObserver
- ChromeTabbedActivityTestRule

- ChromeTabbedActivity
- ModalDialogManager
- ChromeTabModalPresenter

ActivityTestRule -> Activity
Activity -> ModalDialogManager
ModalDialogManager -> ChromeTabModalPresenter
TestObserver ..> Activity.ToolbarManager.ToolbarLayout.LocationBar(UrlFocusChangeListener)


ModalDialogManager.Presenter <|-- TabModalPresenter
TabModalPresenter <|-- ChromeTabModalPresenter


## Setup:
-

## Helpers:
- ModalDialogTestUtils
- createDialog
- checkBrowserControls
  - Check if all tabs are obscured
  - Check if menu button is enabled

## Important locations:
- ChromeActivityTestRule.startMainActivityFromIntent
  - A few calls that wait and verify for some state.

## Questions:
- tab_modal_dialog_container is an importanta view.
  - What is the view structure?
- What are the other tests that use onView and hasDescendant?
