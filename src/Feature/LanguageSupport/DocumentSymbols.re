open EditorCoreTypes;
open Oni_Core;
open Oni_Core.Utility;

type provider = {
  handle: int,
  selector: Exthost.DocumentSelector.t,
};

type symbol = {
  uniqueId: string,
  name: string,
  detail: string,
  kind: Exthost.SymbolKind.t,
  range: CharacterRange.t,
  selectionRange: CharacterRange.t,
};

type t = list(Tree.t(symbol, symbol));

let rec extHostSymbolToTree:
  Exthost.DocumentSymbol.t => Tree.t(symbol, symbol) =
  extSymbol => {
    let children = extSymbol.children |> List.map(extHostSymbolToTree);

    let symbol = {
      // TODO:
      uniqueId: extSymbol.name,
      name: extSymbol.name,
      detail: extSymbol.detail,
      kind: extSymbol.kind,
      range: extSymbol.range |> Exthost.OneBasedRange.toRange,
      selectionRange: extSymbol.range |> Exthost.OneBasedRange.toRange,
    };

    if (children == []) {
      Tree.leaf(symbol);
    } else {
      Tree.node(~children, symbol);
    };
  };

type model = {
  providers: list(provider),
  bufferToSymbols: IntMap.t(list(Tree.t(symbol, symbol))),
};

let initial = {providers: [], bufferToSymbols: IntMap.empty};

[@deriving show]
type msg =
  | DocumentSymbolsAvailable({
      bufferId: int,
      symbols: list(Exthost.DocumentSymbol.t),
    });

let update = (msg, model) => {
  switch (msg) {
  | DocumentSymbolsAvailable({bufferId, symbols}) =>
    let symbolTrees = symbols |> List.map(extHostSymbolToTree);
    let bufferToSymbols =
      IntMap.add(bufferId, symbolTrees, model.bufferToSymbols);
    {...model, bufferToSymbols};
  };
};

let get = (~bufferId, model) => {
  IntMap.find_opt(bufferId, model.bufferToSymbols);
};

let register = (~handle: int, ~selector, model) => {
  ...model,
  providers: [{handle, selector}, ...model.providers],
};

let unregister = (~handle: int, model) => {
  ...model,
  providers: model.providers |> List.filter(prov => prov.handle != handle),
};

let sub = (~buffer, ~client, model) => {
  let toMsg = symbols => {
    DocumentSymbolsAvailable({
      bufferId: Oni_Core.Buffer.getId(buffer),
      symbols,
    });
  };
  //
  model.providers
  |> List.filter(({selector, _}) =>
       selector |> Exthost.DocumentSelector.matchesBuffer(~buffer)
     )
  |> List.map(({handle, _}) => {
       prerr_endline(
         "CALLING DOC SYMBOLS FOR HANDLE: " ++ string_of_int(handle),
       );
       Service_Exthost.Sub.documentSymbols(~handle, ~buffer, ~toMsg, client);
     })
  |> Isolinear.Sub.batch;
};
