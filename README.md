# Process-Coordination
 * [作業目的] 利用pthread的方式來練習process coordination的觀念。
 * [作業內容] 在教授的討論室外面，有5張位置讓要詢問問題的同學先坐著等候。<br/>
 同學依照FCFS的順序，進入討論室與助教討論10~30 milliseconds，然後再與教授討論50~100 milliseconds完成這次的請益。<br/>
 如果討論室外面的位置滿了，此時新到的同學，就需要先離開30~50 milliseconds去看影片，然後再回來看有沒有空位。<br/>
 並且假設同學到達的間隔時間是5~10 milliseconds。
