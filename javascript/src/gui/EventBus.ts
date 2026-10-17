import mitt from 'mitt';
import type {Emitter} from "mitt";

// Event bus singleton.
export class EventBus {
  private static _emitter: Emitter<Record<string, unknown>>;
  
  public static get() : Emitter<Record<string, unknown>> {
    if (EventBus._emitter == undefined) {    
      EventBus._emitter = mitt();
    }
    return EventBus._emitter;
  }
}
