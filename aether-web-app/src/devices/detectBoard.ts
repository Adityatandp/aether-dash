import type { HelloPayload } from '../protocol/types'

export interface DetectedBoard {
  boardId: string
  layoutClass: HelloPayload['layout_class']
  firmware: string
  width: number
  height: number
}

export function boardFromHello(hello: HelloPayload): DetectedBoard {
  return {
    boardId: hello.board_id,
    layoutClass: hello.layout_class,
    firmware: hello.fw,
    width: hello.display.w,
    height: hello.display.h,
  }
}
