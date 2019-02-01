open Revery;
open Revery.Core;
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
  let formatTime = seconds => {
    let minutes = seconds / 60;
    let remainingSeconds = seconds mod 60;

    let paddedRemaingSeconds =
      remainingSeconds < 10 ?
        "0" ++ string_of_int(remainingSeconds) :
        string_of_int(remainingSeconds);

    string_of_int(minutes) ++ ":" ++ paddedRemaingSeconds;
  };

  let component = React.component("Countdown");

  let make = (~countdownTime, ~onStop) =>
    component(slots => {
      let (count, setCount, slots) = React.Hooks.state(countdownTime, slots);

      let (maybeStopInterval, setMaybeStopInterval, slots) =
        React.Hooks.state(None, slots);

      let slots =
        React.Hooks.effect(
          OnMount,
          () => {
            switch (maybeStopInterval) {
            | Some(stopInterval) => stopInterval()
            | None => ()
            };

            setMaybeStopInterval(
              Some(
                Revery_Core.Tick.interval(
                  _ => setCount(count - 1),
                  Revery_Core.Time.Seconds(1.),
                ),
              ),
            );

            Some(() => ());
          },
          slots,
        );

      let _slots: React.Hooks.empty =
        React.Hooks.effect(
          If((current, previous) => current != previous, count),
          () => {
            switch (maybeStopInterval) {
            | Some(stopInterval) => stopInterval()
            | None => ()
            };

            if (count > 0) {
              setMaybeStopInterval(
                Some(
                  Revery_Core.Tick.interval(
                    _ => setCount(count - 1),
                    Revery_Core.Time.Seconds(1.),
                  ),
                ),
              );
            } else {
              onStop();
            };

            Some(() => ());
          },
          slots,
        );

      <Text style=textStyle text={formatTime(count)} />;
    });

  let createElement = (~children as _, ~countdownTime, ~onStop, ()) =>
    React.element(make(~countdownTime, ~onStop));
};

type state =
  | Started
  | Stopped;

module Pomodoro = {
  let component = React.component("Pomodoro");

  let make = () =>
    component(slots => {
      let (state, setState, _slots: React.Hooks.empty) =
        React.Hooks.state(Stopped, slots);

      let stop = () => setState(Stopped);
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
         | Started => <Countdown countdownTime=10 onStop=stop />
         }}
      </View>;
    });

  let createElement = (~children as _, ()) =>
    React.element(make());
};

let init = app => {
  let win =
    App.createWindow(
      app,
      "Welcome to Revery!",
      ~createOptions=
        Revery.Core.Window.{...defaultCreateOptions, resizable: false, decorated: false},
    );

  let render = () => <Pomodoro />;

  UI.start(win, render);
};

App.start(init);

/* {string_of_int(count) ++ "Remaining time " ++ string_of_int(count)} */

/* <Text style=textStyle text="Remaining time" /> */
