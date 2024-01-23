-- Fetch players from backend on load
-- Delete player from backend first then delete player from frontend on success
-- modify player from backend first then modify player from frontend on success
-- modify player from backend first then modify player from frontend on success
-- add player to backend first then add player to frontend on success


module Main exposing (..)

import Browser
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (onCheck, onClick, onInput, onSubmit)
import Http
import Json.Decode as Decode exposing (Decoder, field, map3)
import Json.Encode as Encode


type alias Player =
    { id : Int
    , name : String
    , isActive : Bool
    }


type alias Model =
    { players : List Player
    , newPlayer : Player
    , baseUrl : String
    , reqStatus : String
    }


type Msg
    = SetName String
    | FetchPlayers (Result Http.Error (List Player))
    | PutPlayerReq Int Bool
    | ModifyPlayer (Result Http.Error Player)
    | PostPlayerReq
    | AddPlayer (Result Http.Error Player)
    | DeletePlayerReq Int
    | DeletePlayer Int (Result Http.Error ())


playerEncoder : Player -> Encode.Value
playerEncoder player =
    Encode.object
        [ ( "id", Encode.int player.id )
        , ( "name", Encode.string player.name )
        , ( "isActive", Encode.bool player.isActive )
        ]


playerDecoder : Decoder Player
playerDecoder =
    map3 Player (field "id" Decode.int) (field "name" Decode.string) (field "isActive" Decode.bool)


playersDecoder : Decoder (List Player)
playersDecoder =
    Decode.list playerDecoder


fetchPlayers : String -> Cmd Msg
fetchPlayers url =
    Http.get
        { url = url
        , expect = Http.expectJson FetchPlayers playersDecoder
        }

postPlayerReq : String -> Player -> Cmd Msg
postPlayerReq url player = 
    Http.post 
        { url = url
        , body = Http.jsonBody (playerEncoder player)
        , expect = Http.expectJson AddPlayer playerDecoder
        }


deletePlayerReq : String -> Int -> Cmd Msg
deletePlayerReq url id = 
    Http.request 
        { method = "DELETE"
        , headers = []
        , url=url++ String.fromInt id
        , body = Http.emptyBody
        , expect = Http.expectWhatever (DeletePlayer id)
        , timeout = Nothing
        , tracker = Nothing
        }


putPlayerReq : String -> Player -> Cmd Msg
putPlayerReq url player = 
    Http.request 
        { method = "PUT"
        , headers = []
        , url = url ++ String.fromInt player.id
        , body = Http.jsonBody (playerEncoder player)
        , expect = Http.expectJson ModifyPlayer playerDecoder
        , timeout = Nothing
        , tracker = Nothing
        }

listLast : List a -> Maybe a
listLast list =
    List.head <| List.reverse list


initPlayer : Int -> Player
initPlayer id =
    Player id "" False


initModel : Model
initModel =
    { players = []
    , newPlayer = initPlayer 0
    , baseUrl = "http://localhost:3001/api/players/"
    , reqStatus = "Loading..."
    }


init : () -> ( Model, Cmd Msg )
init _ =
    ( initModel
    , fetchPlayers initModel.baseUrl
    )


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        SetName word ->
            let
                oldName = model.newPlayer
                newName = { oldName | name = word }
            in
            ( { model | newPlayer = newName }, Cmd.none )

        FetchPlayers data ->
            case data of
                Ok players ->
                    ( { model | players = players, reqStatus = "", newPlayer = initPlayer (List.length players + 1) }, Cmd.none )
                Err _ ->
                    ( { model | reqStatus = "An error has occurred!!!" }, Cmd.none )


        PostPlayerReq ->
            let
                player = model.newPlayer
                postCmd=postPlayerReq model.baseUrl player
            in
                (model, postCmd)

        AddPlayer data ->
            case data of
                Ok player ->
                    ( { model | players = model.players ++ [player], newPlayer = initPlayer (List.length model.players + 1) }, Cmd.none )
                Err _ ->
                    ( { model | reqStatus = "An error has occurred!!!" }, Cmd.none )

        PutPlayerReq id status ->
            let
                maybePlayer = List.head <| List.filter (\player -> player.id == id ) model.players
                putCmd = 
                    case maybePlayer  of
                        Just player ->
                            putPlayerReq model.baseUrl { player | isActive = status}
                        Nothing ->
                            Cmd.none
            in
                (model, putCmd)

        ModifyPlayer data ->
            case data of
                Ok modifiedPlayer ->
                    ( { model | players = List.map (\player -> if player.id == modifiedPlayer.id then modifiedPlayer else player) model.players }, Cmd.none )
                Err _ ->
                    ( { model | reqStatus = "An error has occurred!!!" }, Cmd.none )

        DeletePlayerReq id ->
            let
                deleteCmd = deletePlayerReq model.baseUrl id
            in
                (model, deleteCmd)

        DeletePlayer id data ->
            case data of
                Ok _ ->
                    ( { model | players = List.filter (\player -> player.id /= id) model.players }, Cmd.none )
                Err _ ->
                    ( { model | reqStatus = "An error has occurred!!!" }, Cmd.none )


view : Model -> Html Msg
view model =
    div []
        [ h1 [] [ text "Elm Exercise: Players CRUD" ]
        , Html.form [ id "submit-player", onSubmit PostPlayerReq ]
            [ input [ id "input-player", type_ "text", value model.newPlayer.name, onInput SetName ] []
            , button [ id "btn-add", type_ "submit" ] [ text "Add Player" ]
            ]
        , ol [ id "players-list" ]
            (List.map (\player ->
                li [ id <| "player-" ++ String.fromInt player.id ]
                    [ div [ class "player-name" ] [ text player.name ]
                    , label [ class "player-status" ]
                        [ input [ type_ "checkbox", class "player-status", checked player.isActive, onCheck <| PutPlayerReq player.id ] []
                        , text "Active"
                        ]
                    , button [ class "btn-delete", onClick <| DeletePlayerReq player.id ] [ text "Delete" ]
                ]
            ) model.players)
        , div [ id "request-status" ] [ text model.reqStatus ]
    ]



main : Program () Model Msg
main =
    Browser.element
        { init = init
        , view = view
        , update = update
        , subscriptions = \_ -> Sub.none
        }
