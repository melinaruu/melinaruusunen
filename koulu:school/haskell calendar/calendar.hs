import Dates
import Data.Map (Map)
import qualified Data.Map as Map
import Data.List (group, groupBy)
import Data.List (intercalate)


-- Data structure to hold events
type EventName = String
type Place = String
type EventCalendar = Map Date (Map EventName Place)

splitString :: String -> [String]
splitString [] = []
splitString ('\'':xs) = let (word, rest) = break (== '\'') xs
                        in word : splitString (drop 1 rest)
splitString str = let (word, rest) = break (`elem` " '") str
                 in if null word
                    then splitString (drop 1 rest)
                    else word : splitString rest

wordsWhen     :: (Char -> Bool) -> String -> [String]
wordsWhen p s =  case dropWhile p s of
                      "" -> []
                      s' -> w : wordsWhen p s''
                            where (w, s'') = break p s'

handleInput :: EventCalendar -> String -> IO EventCalendar
handleInput calendar input = do
    putStrLn ("> " ++ input)
    let inputWords = splitString input
    case inputWords of
        "Event" : eventName : "happens" : "at" : place : "on" : dateString -> do
            let splitDate = wordsWhen (=='-') (unwords dateString)
            let formattedDate = makeDateFromInput splitDate
            case formattedDate of
                Just date -> do
                    putStrLn "Ok"
                    return $ addEvent calendar eventName place date
                Nothing   -> do
                    putStrLn "Bad date"
                    return calendar
        "Tell" : "me" : "about" : eventName -> do
            let event = unwords eventName
            putStrLn $ showEventDetails calendar event
            return calendar
        "What" : "happens" : "on" : dateString -> do
            let splitDate = wordsWhen (=='-') (head dateString)
            let formattedDate = makeDateFromInput splitDate
            case formattedDate of
                Just date -> do
                    putStrLn $ showEventsOnDate calendar date
                    return calendar
                Nothing   -> do
                    putStrLn "Bad date"
                    return calendar
        "What" : "happens" : "at" : place -> do
            let eventPlace = unwords place
            putStrLn $ showEventsAtPlace calendar eventPlace
            return calendar
        _ -> do
            putStrLn "I do not understand that. I understand the following:"
            putStrLn "*Event <name> happens at <place> on <date>"
            putStrLn "*Tell me about <eventname>"
            putStrLn "*What happens on <date>"
            putStrLn "*What happens at <place>"
            putStrLn "*Quit"
            return calendar

addEvent :: EventCalendar -> EventName -> Place -> Date -> EventCalendar
addEvent calendar eventName place date =
    let eventMap = Map.singleton eventName place
        updatedCalendar = case getEventDetails calendar eventName of
            Just (_, existingDate) ->
                let updatedDateMap = Map.insert date eventMap $ Map.delete existingDate calendar
                in if existingDate == date
                    then updatedDateMap
                    else updatedDateMap
            Nothing -> Map.insertWith Map.union date eventMap calendar
    in updatedCalendar

-- Function to display event details
showEventDetails :: EventCalendar -> EventName -> String
showEventDetails calendar eventName =
    case getEventDetails calendar eventName of
        Just (place, date) -> "Event " ++ eventName ++ " happens at " ++ place ++ " on " ++ show date
        Nothing            -> "I do not know of such event"

-- Function to get details of a specific event
getEventDetails :: EventCalendar -> EventName -> Maybe (Place, Date)
getEventDetails calendar eventName =
    let allEvents = Map.elems calendar
        eventDetails = foldr findEvent [] allEvents
    in case eventDetails of
        [] -> Nothing
        (place, date) : _ -> Just (place, date)
    where
        findEvent :: Map EventName Place -> [(Place, Date)] -> [(Place, Date)]
        findEvent events acc =
            case Map.lookup eventName events of
                Just place -> (place, getDateFromEvents calendar eventName) : acc
                Nothing    -> acc

        getDateFromEvents :: EventCalendar -> EventName -> Date
        getDateFromEvents cal name =
            case Map.keys $ Map.filter (Map.member name) cal of
                []       -> error "Event not found in the calendar"
                (d : _)  -> d

-- Function to display events on a particular date
showEventsOnDate :: EventCalendar -> Date -> String
showEventsOnDate calendar date =
    case Map.lookup date calendar of
        Just events ->
            if Map.null events
                then "No events on " ++ show date
                else intercalate "\n" $ map (\(eventName, place) -> "Event " ++ eventName ++ " happens on " ++ show date) (Map.toList events)
        Nothing     -> "Nothing that I know of"

-- Function to display events at a specific place
showEventsAtPlace :: EventCalendar -> Place -> String
showEventsAtPlace calendar place =
    let filteredEvents = Map.foldrWithKey (\date events acc -> if place `elem` Map.elems events then Map.assocs events : acc else acc) [] calendar
    in if null filteredEvents
        then "Nothing that I know of"
        else intercalate "\n" $ map (formatEvents place) filteredEvents

formatEvents :: Place -> [(EventName, Place)] -> String
formatEvents place events =
    intercalate "\n" [ "Event " ++ name ++ " happens at " ++ place' | (name, place') <- events, place' == place ]

-- Function to get the event details for a specific place on a given date
showEventAtPlace :: EventCalendar -> Date -> Place -> String
showEventAtPlace calendar date place =
    case Map.lookup date calendar of
        Just events -> case Map.lookup place events of
            Just eventName -> eventName ++ " happens at " ++ place
            Nothing        -> ""
        Nothing     -> ""

-- Helper function to create Date from user input
makeDateFromInput :: [String] -> Maybe Date
makeDateFromInput [year, month, day] = makeMaybeDate (read year) (read month) (read day)
makeDateFromInput _ = Nothing

eventLoop :: EventCalendar -> IO EventCalendar
eventLoop calendar = do
    input <- getLine
    if input /= "Quit"
        then do
            newCalendar <- handleInput calendar input
            eventLoop newCalendar
        else
            return calendar

main :: IO ()
main = do
    initialCalendar <- eventLoop Map.empty
    putStrLn "> Quit"
    putStrLn "Bye"