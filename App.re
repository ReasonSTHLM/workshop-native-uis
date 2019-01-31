open Revery;
open Revery.Core;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let textStyle = Style.make(~fontFamily="Lato-Regular.ttf", ~fontSize=20, ());

module SimpleButton = {
  let component = React.component("SimpleButton");

  let make = (~onClick, ~text) =>
    component((_slots: React.Hooks.empty) => {
      let wrapperStyle =
        Style.make(
          ~backgroundColor=Color.rgba(1., 1., 1., 0.1),
          ~border=Style.Border.make(~width=2, ~color=Colors.white, ()),
          ~margin=16,
          (),
        );

      let textHeaderStyle =
        Style.make(
          ~color=Colors.white,
          ~fontFamily="Lato-Regular.ttf",
          ~fontSize=20,
          ~margin=4,
          (),
        );

      <Clickable onClick>
        <View style=wrapperStyle> <Text style=textHeaderStyle text /> </View>
      </Clickable>;
    });

  let createElement = (~children as _, ~onClick, ~text, ()) =>
    React.element(make(~onClick, ~text));
};

module Countdown = {
  let component = React.component("Countdown");

  let make = (~countdownTime) =>
    component(slots => {
      let (count, setCount, slots) = React.Hooks.state(countdownTime, slots);

      let (maybeStopInterval, setMaybeStopInterval, slots) =
        React.Hooks.state(None, slots);

      let slots =
        React.Hooks.effect(
          React.Hooks.Effect.Always,
          () => {
            print_endline("lalkdfasfas");
            
            Some(
              () => {
                print_endline("lalala");

                setMaybeStopInterval(
                  Some(
                    Revery_Core.Tick.interval(
                      _ => setCount(count - 1),
                      Revery_Core.Time.Seconds(0.),
                    ),
                  ),
                )
              }
            )
            },
          slots,
        );

      let _slots: React.Hooks.empty =
        React.Hooks.effect(
          React.Hooks.Effect.If((current, _) => current <= 0, count),
          () => maybeStopInterval,
          slots,
        );

      <Text style=textStyle text={string_of_int(count)} />;
    });

  let createElement = (~children as _, ~countdownTime, ()) =>
    React.element(make(~countdownTime));
};

module Pomodoro = {
  let component = React.component("Pomodoro");

  let make = (~initialState, ~countdownTime) =>
    component(slots => {
      let (count, setCount, countdownSlots: React.Hooks.empty) =
        React.Hooks.state(initialState, slots);

      let decrement = () => setCount(count - 1);
      let start = () => setCount(countdownTime);

      print_endline(string_of_int(count));

      <View
        style={Style.make(
          ~position=LayoutTypes.Absolute,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~alignItems=LayoutTypes.AlignCenter,
          ~bottom=0,
          ~top=0,
          ~left=0,
          ~right=0,
          (),
        )}>
        {switch (count) {
         | 0 =>
           <View
             style={Style.make(
               ~flexDirection=Row,
               ~alignItems=AlignFlexEnd,
               (),
             )}>
             <Text style=textStyle text="Start the Pomodoro" />
             <View> <SimpleButton onClick=start text="Start" /> </View>
           </View>
         | _ =>
           <View
             style={Style.make(
               ~flexDirection=Row,
               ~alignItems=AlignFlexEnd,
               (),
             )}>
             <Countdown countdownTime=10 />
           </View>
         }}
      </View>;
    });

  let createElement = (~children as _, ~initialState=0, ~countdownTime=10, ()) =>
    React.element(make(~initialState, ~countdownTime));
};

let init = app => {
  let win = App.createWindow(app, "Welcome to Revery!");

  let render = () => <Pomodoro initialState=0 />;

  UI.start(win, render);
};

App.start(init);

/* {string_of_int(count) ++ "Remaining time " ++ string_of_int(count)} */

/* <Text style=textStyle text="Remaining time" /> */
