import React, {useEffect, useCallback, useState} from 'react';
import useWebSocket, { ReadyState } from 'react-use-websocket';

import './App.css';

const WS_URL = 'ws://192.168.170.137:7000';


function App() {

  const { sendMessage, lastMessage, readyState, getWebSocket } = useWebSocket(WS_URL);
  const [messageHistory, setMessageHistory] = useState([]);
  const [p0gU,setP0gU] = useState([]);


  const on = useCallback(() => sendMessage('on'), []);
  const off = useCallback(() => sendMessage('off'), []);
  useEffect(() => {
    if(lastMessage!== null){
      let data = lastMessage.data
      let abc = JSON.parse(data)
      setMessageHistory((prev)=>[...prev,abc])
      // console.log(typeof messageHistory[0])
      console.log(messageHistory)
    }
    // if (lastMessage !== null) {
    //   setMessageHistory((prev) => [...prev,lastMessage]);
    //   console.log(messageHistory)
    // }
  }, [lastMessage, setMessageHistory]);
  // useEffect(()=>{
  //   sendMessage('hola amigoss')
  // },[])
  // console.log(d1)
  messageHistory.forEach((val,i)=>{
    val.client_1.forEach(({beacon,rssi},j)=>{
      let a = beacon == "p0gU"
      if(a){
        let abc = Math.pow(10,((-69 -(rssi))/(10 * 2)))
        // console.log(abc)
        setP0gU((prev)=>[...prev,abc])
        console.log(p0gU)
      }
    })
  })
  return (
    <div>Hello WebSockets!
      <button onClick={on}>On</button>
      <button onClick={off}>off</button><br/>
      {/*{messageHistory ? <span>message History: {messageHistory[0].data}</span> : <div>pubg mobile</div>}*/}
      {/*{console.log(messageHistory[0].client_1)}*/}
      {/*{messageHistory?<span>MessageHistory : </span>:null}*/}
      {/*{messageHistory.forEach()}*/}
      {/*{messageHistory.map((inner,i)=>{*/}
      {/*  // {console.log(typeof inner)}*/}
      {/*  return <ol key={i}>{Object.values(inner).map(({beacon,rssi},j)=>{*/}
      {/*    return <li key={j}>beacon - {beacon}  rssi - {rssi}</li>*/}
      {/*  })}</ol>*/}
      {/*})}*/}
      {messageHistory.map((innerObject,i)=>{
        return(
            <ol key={i}>
              {innerObject.client_1.map(({beacon,rssi},j)=>(
                  <li key={j}>
                    Beacon: {beacon} - RSSI: {rssi}
                  </li>
              ))}
            </ol>
        )
      })}
    </div>
  );
}

export default App;
