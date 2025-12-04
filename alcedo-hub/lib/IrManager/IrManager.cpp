/**
 * @file IrManager.cpp
 * @brief 赤外線送受信管理の実装
 * @author Alcedo Pebble Project
 */

#include "IrManager.h"

// 受信バッファサイズ（エアコン等の長い信号に対応するため大きめに確保）
const uint16_t kCaptureBufferSize = 1024;
// タイムアウト (ミリ秒)
const uint8_t kTimeout = 50;
// 許容誤差 (%)
// Renamed to avoid conflicting declaration in other translation units
const uint16_t kIrTolerance = 0; // kToleranceだと競合するため

/**
 * @brief コンストラクタ
 * @param sendPin 送信LED接続ピン (GPIO)
 * @param recvPin 受信モジュール接続ピン (GPIO)
 */
IrManager::IrManager(uint16_t sendPin, uint16_t recvPin) 
    : _sendPin(sendPin), 
      _recvPin(recvPin), 
      _irsend(sendPin), 
      _irrecv(recvPin, kCaptureBufferSize, kTimeout, true), // saveBuffer=true
      _dataReady(false)
{
}

/**
 * @brief セットアップ
 */
void IrManager::setup() {
    _irsend.begin();
    _irrecv.enableIRIn(); // 受信開始
    Serial.println("[IR] Hardware Initialized.");
}

/**
 * @brief ループ処理
 * IR受信の監視を行う
 */
void IrManager::loop() {
    // 受信データがあるかチェック
    if (_irrecv.decode(&_results)) {
        // ノイズ対策: 短すぎる信号 (10ビット以下) は無視
        if (_results.bits > 10) {
            _dataReady = true;
            // ここでは resume() しない。データ取得後に resume する
        } else {
            _irrecv.resume(); // ノイズだった場合は即次へ
        }
    }
}

/**
 * @brief NECフォーマットで送信
 */
void IrManager::sendNEC(uint64_t data, uint16_t bits) {
    // NEC送信用 (repeat=0)
    Serial.printf("[IR] Sending NEC: 0x%llX (%d bits)\n", data, bits);
    _irsend.sendNEC(data, bits);
    
    // 送信中は受信できないため、送信後に受信を再開する
    _irrecv.enableIRIn(); 
}

/**
 * @brief Rawデータ送信（汎用）
 * @param buf タイミングデータの配列
 * @param len 配列長
 * @param freq 周波数 (kHz)
 */
void IrManager::sendRaw(const uint16_t* buf, uint16_t len, uint16_t freq) {
    Serial.printf("[IR] Sending RAW: len=%d, freq=%d\n", len, freq);
    _irsend.sendRaw(buf, len, freq);
    
    // 送信後に受信再開
    _irrecv.enableIRIn();
}

/**
 * @brief 有効な受信データが待機中か確認
 */
bool IrManager::hasReceivedData() {
    return _dataReady;
}

/**
 * @brief 最後に受信したデータを16進数文字列で取得
 * 取得と同時に受信フラグをクリアし、次の受信を開始する
 */
String IrManager::getLastReceivedHex() {
    if (!_dataReady) return "";

    // 64bit数値を16進数文字列へ変換
    // ArduinoのStringコンストラクタではuint64_tを扱えない場合があるためsprintfを使用
    char hexBuf[32];
    
    // protocol判定結果を付与する場合
    // String proto = typeToString(_results.decode_type);
    
    // 単純なHEXデータ化
    // serializableな形式にするためにここでは値のみを返す
    sprintf(hexBuf, "0x%llX", _results.value);
    
    String retData = String(hexBuf);

    // フラグクリアして受信再開
    _dataReady = false;
    _irrecv.resume();

    return retData;
}

/**
 * @brief 直近の受信プロトコル名を取得 (デバッグ用)
 */
String IrManager::getLastProtocolName() {
    return typeToString(_results.decode_type);
}