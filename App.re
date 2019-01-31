open Revery;
open Revery.Core;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let textStyle = Style.[fontFamily("Lato-Regular.ttf"), fontSize(20)];

module SimpleButton = {
  let component = React.component("SimpleButton");

  let make = (~onClick, ~text) =>
    component((_slots: React.Hooks.empty) => {
      let wrapperStyle =
        Style.[backgroundColor(Color.rgba(1., 1., 1., 0.1)), margin(16)];

      <Clickable onClick>
        <View style=wrapperStyle> <Text style=textStyle text /> </View>
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
          OnMount,
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
                );
              },
            );
          },
          slots,
        );

      let _slots: React.Hooks.empty =
        React.Hooks.effect(
          React.Hooks.Effect.Always,
          () => {
            print_endline("Aasdf ASDF ");

            maybeStopInterval;
          },
          slots,
        );

      <Text style=textStyle text={string_of_int(count)} />;
    });

  let createElement = (~children as _, ~countdownTime, ()) =>
    React.element(make(~countdownTime));
};

type state =
  | Started
  | Stopped;

module Pomodoro = {
  let component = React.component("Pomodoro");

  let make = (~initialState, ~countdownTime) =>
    component(slots => {
      let (state, setState, _slots: React.Hooks.empty) =
        React.Hooks.state(Stopped, slots);

      /* let stop = () => setState(Stopped); */
      let start = () => setState(Started);

      <View
        style=Style.[
          position(`Absolute),
          justifyContent(`Center),
          alignItems(`Center),
          bottom(0),
          top(0),
          left(0),
          right(0),
        ]>
        {switch (state) {
         | Stopped =>
           <View style=Style.[flexDirection(`Row), alignItems(`FlexEnd)]>
             <Text style=textStyle text="Start the Pomodoro" />
             <View> <SimpleButton onClick=start text="Start" /> </View>
           </View>
         | Started =>
           <View style=Style.[flexDirection(`Row), alignItems(`FlexEnd)]>
             <Countdown countdownTime=8 />
           </View>
         }}
      </View>;
    });

  let createElement = (~children as _, ~initialState=0, ~countdownTime=10, ()) =>
    React.element(make(~initialState, ~countdownTime));
};

let init = app => {
  let win = App.createWindow(app, "Welcome to Revery!");

  let render = () => <Pomodoro />;

  UI.start(win, render);
};

App.start(init);

/* {string_of_int(count) ++ "Remaining time " ++ string_of_int(count)} */

/* <Text style=textStyle text="Remaining time" /> */
