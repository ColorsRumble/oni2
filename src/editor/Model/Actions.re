/*
 * Actions.re
 *
 * Encapsulates actions that can impact the editor state
 */

open Oni_Core.Types;
open Oni_Extensions;

type t =
  | BufferDelete(BufferNotification.t)
  | BufferEnter(BufferNotification.t)
  | BufferUpdate(BufferUpdate.t)
  | BufferWritePost(BufferNotification.t)
  | TablineUpdate(Tabline.tabs)
  | TextChanged(TextChanged.t)
  | TextChangedI(TextChanged.t)
  | ChangeMode(Mode.t)
  | CursorMove(BufferPosition.t)
  | SetEditorFont(EditorFont.t)
  | SetEditorSize(EditorSize.t)
  | RecalculateEditorView
  | CommandlineShow(commandline)
  | CommandlineHide(commandline)
  | CommandlineUpdate((int, int))
  | WildmenuShow(wildmenu)
  | WildmenuHide(wildmenu)
  | WildmenuSelected(int)
  | ShowEditor
  | ShowHome
  | EditorScroll(float)
  | EditorScrollToCursorCentered
  | EditorScrollToCursorTop
  | EditorScrollToCursorBottom
  | EditorMoveCursorToTop(Cursor.move)
  | EditorMoveCursorToMiddle(Cursor.move)
  | EditorMoveCursorToBottom(Cursor.move)
  | SyntaxHighlightColorMap(ColorMap.t)
  | SyntaxHighlightTokens(TextmateClient.TokenizationResult.t)
  | MenuRegisterEffects(Effects.t(t))
  | MenuSearch(string)
  | MenuOpen((UiMenu.menuType, UiMenu.commandFactory(t)))
  | MenuClose
  | MenuSelect
  | MenuPosition(int)
  | MenuUpdate((UiMenu.menuType, list(UiMenu.command)))
  | SetInputControlMode(Input.controlMode)
  | StatusBarAddItem(StatusBarModel.Item.t)
  | StatusBarDisposeItem(int)
  | Noop;
