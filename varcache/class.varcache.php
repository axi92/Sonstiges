<?php
/* class.varcache.php
.---------------------------------------------------------------------------.
|  Software: varCache - PHP Cache Class                                     |
|   Version: 2                                                              |
|      Site: http://jspit.de/?page=cache                                    |
| ------------------------------------------------------------------------- |
| Copyright (c) 2013, Peter Junk. All Rights Reserved.                      |
| ------------------------------------------------------------------------- |
|   License: Distributed under the Lesser General Public License (LGPL)     |
|            http://www.gnu.org/copyleft/lesser.html                        |
| This program is distributed in the hope that it will be useful - WITHOUT  |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     |
| FITNESS FOR A PARTICULAR PURPOSE.                                         |
'---------------------------------------------------------------------------'
 * 2013-03-25
 */
interface cacheInterface {
  public function get($name=null);
  public function set($name, $value);
  public function delete($name=null);
  public function exists($name);
}

interface cacheInterfaceAll extends cacheInterface {
  public function lock() ;
  public function unlock() ;
}

class varCache implements cacheInterfaceAll{
  //Variablen cachen
  const FILE_BUF = 32768; //max. Filegröße 32k
  
  private $cacheFileName = 'varcache.dat';
  private static $cacheFiles = array();

  private $fp = null;
  
  
  public function __construct($fileName = null)
  {
    if($fileName !== null) $this->cacheFileName = $fileName;
    //wenn Datei noch nicht existiert -> erstellen
    if(! file_exists($this->cacheFileName)) {
      //datei erstellen
      file_put_contents($this->cacheFileName ,json_encode(array()));
    } 
    //
    if(in_array(realpath($this->cacheFileName),self::$cacheFiles)) {
      throw new Exception("Fehler: Zwei Instanzen mit der selben Dateibasis ist nicht zulässig");
    }
    else {
      self::$cacheFiles[] = realpath($this->cacheFileName);  
    }

  }
  
  public function __destruct() {
    $this -> unlock();
  }
  
  /*
   * Liefert Wert bei übergabe name, Array mit allen Werten ohne Name
   * liefert null, wenn name nicht existiert
   */
  public function get($name=null) {
    $cacheArray = $this -> getCache();
    if($name === null) return $cacheArray; //All
    if(isset($cacheArray[$name])) return $cacheArray[$name];
    return null;
  }

  public function __get($name) {
    $cacheArray = $this -> getCache();
    if(isset($cacheArray[$name])) return $cacheArray[$name];
    return null;
  }
  
  public function exists($name) {
    $cacheArray = $this -> getCache(); 
    return isset($cacheArray[$name]);
  }
  
  /*
   * wird gerufen, wenn Attribut mit isset oder empty abgefragt wird
   * return true or false
   */
  public function __isset($name){
    return $this->exists($name);
  }
  
  /*
   * Setzt Wert
   */
  public function __set($name, $value) {
    $this -> updateCache($name ,$value);
  }
  
  /*
   * Speichert $value unter den Namen $name
   * return true bei Erfolg, false bei Fehler
   */
  public function set($name, $value) {
    return $this -> updateCache($name ,$value);
  }
  
  /*
   * lock: öffnet und verriegelt Datei exklusiv
   * return false wenn Datei nicht geöffnet werden kann,
   * keine Freigabe bekommt oder schon verriegelt ist.
   */
  public function lock() {
    if($this -> fp === null) {
      $fp = fopen($this->cacheFileName, 'r+');
      if($fp) {
        if (flock($fp, LOCK_EX)) { // exklusive Sperre
          $this -> fp = $fp;
          return true;
        }
      }
    }
    return false; //Error
  }
   
  public function unlock(){
    if($this->fp) {
      $rl = flock($this->fp, LOCK_UN); // Sperre freigeben
      $rb = fclose($this->fp);
      $this->fp = null;
      return $rb && $rl;
    }
    return false;    
  }
  
  /*
   * isLock() liefert true wenn Interface lock ist
   */
  public function isLock() {
    return $this->fp !== null;
  }
  
  /*
   * unset($c -> name)
   */
  public function __unset($name) {
    $this -> delete($name);
  }
  
  /*
   * löscht alle oder nur das element
   * return true or false
   */
  public function delete($name=null){
    $ret = false;
    $local = $this->fp === null;
    if($local) $lockOk = $this -> lock();
    else $lockOk = true; //lock per Methode
    if($lockOk) {
      $fp = $this -> fp;
      if($name === null) {
        //komplett löschen
          $cacheArray = array();
          $ret = true;
      }
      else {
        rewind($fp);
        $cacheArray = json_decode(fread($fp,self::FILE_BUF),true);
        if(isset($cacheArray[$name])) {
          //Element löschen
          unset($cacheArray[$name]);
          $ret = true;
        }
      }
      if($ret) { //cache verändert
        rewind($fp);
        ftruncate($fp, 0);
        fwrite($fp,json_encode($cacheArray));
      }
      if($local) $this->unlock();
    }  
    return $ret;
  }
   
  /*
   * return cache-array or null
   */
  private function getCache() {
    $local = $this->fp === null;
    if($local) $lockOk = $this -> lock();
    else $lockOk = true; //lock per Methode
    
    $cacheArray = null;
    if($lockOk) {
      rewind($this->fp);
      $cacheArray = json_decode(fread($this->fp,self::FILE_BUF),true);
      if($local) $this->unlock();  
    }  
    return $cacheArray;
  }

  /*
   * update cache-array, return true or false
   */
  private function updateCache($name ,$value){
    $local = $this->fp === null;
    if($local) $lockOk = $this -> lock();
    else $lockOk = true; //lock per Methode
    if($lockOk) {
      $fp = $this->fp;
      rewind($fp);
      $cacheArray = json_decode(fread($fp,self::FILE_BUF),true);
      $cacheArray[$name] = $value;
      rewind($fp);
      ftruncate($fp, 0);
      fwrite($fp,json_encode($cacheArray));
      if($local) $this->unlock();  
      return true;
    }  
    return false;
  }

}
?>
