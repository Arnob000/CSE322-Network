import java.io.*;
import java.net.Socket;
import java.nio.file.DirectoryIteratorException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class ServerThread implements Runnable{
    String username;
    Socket socket;
    ObjectOutputStream out;
    ObjectInputStream in;
    HashMap<String, Info> clientList;
    HashMap<Long,String> fileList;
    Info clientInfo;

    String filepath;
    final int MAX_BUFFER_SIZE=5000;
    final int MIN_CHUNCK_SIZE=10;
    final int MAX_CHUNCK_SIZE=100;
    byte[] byteArr;
    int numOfBytes;
    Thread t;

    ServerThread(Socket s, HashMap<String , Info> list, String path) throws IOException {
        clientList=list;
        socket=s;
        filepath=path;
        out=new ObjectOutputStream(socket.getOutputStream());
        in =new ObjectInputStream(socket.getInputStream());
        fileList=Server.fileList;
        byteArr=null;

        t=new Thread(this);
        t.start();
    }

    public void download(String fhost, String type) throws IOException, ClassNotFoundException{
        String fname="";
        String outputmsg="";

        outputmsg="File Name:";
        out.writeObject(outputmsg);
        fname=(String) in.readObject();                          

        String fpath=filepath+"/"+fhost.toUpperCase()+"/"+type+"/"+fname;
        byteArr=new byte[MAX_CHUNCK_SIZE];
        File file=new File(fpath);
        FileInputStream fi=new FileInputStream(file);
        
        outputmsg=""+MAX_CHUNCK_SIZE;
        out.writeObject(outputmsg);
        //gfg
        while ((numOfBytes=fi.read(byteArr))!=-1){
            outputmsg="Chuncksize "+numOfBytes;
            out.writeObject(outputmsg);
            out.writeObject(byteArr);
            System.out.println("Chuncksize "+numOfBytes+" sent");
            byteArr=new byte[MAX_CHUNCK_SIZE]; 
        }
        fi.close();
        outputmsg="File sent";
        out.writeObject(outputmsg);        
    }

    public void run(){
        Boolean flag=true;
        String inputmsg="";
        String outputmsg="";
        File file1 = null;
        File file2 = null;
        try {
            out.writeObject("Enter Name: ");
            username=(String)in.readObject();
            username=username.toUpperCase();

            if(clientList.containsKey(username)){
                clientInfo=clientList.get(username);
                if(clientInfo.online){
                    out.writeObject("User "+username+" is already connected\nEnter correct username");
                    flag=false;
                    username="Unknown";
                }
                else {
                    out.writeObject("User " + username + " is connected again");
                    System.out.println("User " + username + " is connected again");
                    clientList.put(username,new Info(username,in, out, socket));
                    clientInfo=clientList.get(username);
                }
            }
            else {
                out.writeObject("User " + username + " is connected");
                System.out.println("User " + username + " is connected");

                clientList.put(username,new Info(username,in, out, socket));
                clientInfo=clientList.get(username);
                //create new dir
                Files.createDirectories(Path.of(filepath + "/" + username));
                Files.createDirectories(Path.of(filepath + "/" + username+"/Public"));
                Files.createDirectories(Path.of(filepath + "/" + username+"/Private"));
                Files.createDirectories(Path.of(filepath + "/" + username+"/Message"));
                file1=new File(filepath + "/" + username+"/Message/readmsg.txt");
                file1.createNewFile();
                file2=new File(filepath + "/" + username+"/Message/unreadmsg.txt");
                file2.createNewFile();

                Thread.sleep(100);
            }


            if(flag){
                while(true){
                    out.writeObject("\nOptions:\n\t1.Client List\n\t2.Uploaded files\n\t3.Downloadable files\n\t4.File Request\n\t5.Unread Messages\n\t6.Upload\n\t7.All Messages\n\t0.Log Out");
                    inputmsg=(String) in.readObject();

                    //list
                    if(inputmsg.contains("1")){
                        outputmsg="Client List-\n";
                        for(Map.Entry<String, Info> clients: clientList.entrySet()){
                            outputmsg+="\t"+clients.getKey()+(clients.getValue().online?"(Online)":"(Offline)")+"\n";
                        }
                        out.writeObject(outputmsg);
                        System.out.println("Client List is sent to "+username);
                    }

                    //uploaded files: download
                    else if(inputmsg.contains("2")){
                        String type="";
                        outputmsg="File Type: 1.Public 2.Private";
                        out.writeObject(outputmsg);
                        inputmsg=(String) in.readObject();
                        if(inputmsg.contains("1"))
                            type="Public";
                        else if(inputmsg.contains("2")){
                            type="Private";
                        }

                        outputmsg="\n"+type+" files-\n";
                        inputmsg=filepath+"/"+username+"/"+type;
                        //oracle doc
                        Path dir = Path.of(inputmsg);
                        try (DirectoryStream<Path> stream = Files.newDirectoryStream(dir)) {
                            for (Path file: stream) {
                                outputmsg+="\t"+file.getFileName()+"\n";
                            }
                        } catch (IOException | DirectoryIteratorException x) {
                            System.out.println(x);
                        }

                        out.writeObject(outputmsg);
                        
                        //downloading                        
                        outputmsg="Do you want to download?\n\t1.Yes\n\t2.No";
                        out.writeObject(outputmsg);
                        inputmsg=(String) in.readObject();
                        if(inputmsg.contains("1")){
                            try{
                                download(username,type); 
                            } catch(Exception e){
                                System.out.println("Exception: "+e);
                                out.writeObject("File not found");
                            }
                        }
                    }

                    //other files: download
                    else if(inputmsg.contains("3")){
                        outputmsg="\nDownloadable files-\n";
                        for(Map.Entry<String, Info> clients: clientList.entrySet()){
                           if(!clients.getKey().equalsIgnoreCase(username)){
                               //oracle doc
                               Path dir = Path.of(filepath+"/"+clients.getKey()+"/Public");
                               try (DirectoryStream<Path> stream = Files.newDirectoryStream(dir)) {
                                   for (Path file: stream) {
                                       outputmsg+="\t"+file.getFileName()+"("+clients.getKey()+")"+"\n";
                                   }
                               } catch (IOException | DirectoryIteratorException x) {
                                   System.out.println(x);
                               }
                           }
                        }

                        out.writeObject(outputmsg);

                        //downloading                        
                        outputmsg="Do you want to download?\n\t1.Yes\n\t2.No";
                        out.writeObject(outputmsg);
                        inputmsg=(String) in.readObject();
                        if(inputmsg.contains("1")){
                            String fhost="";                            
                            outputmsg="File Host:";
                            out.writeObject(outputmsg);
                            fhost=(String) in.readObject();

                            try{
                                download(fhost,"Public");   
                            } catch(Exception e){
                                System.out.println("Exception: "+e);
                                out.writeObject("File not found");
                            }
                        }
                    }

                    //request
                    else if(inputmsg.contains("4")){
                        String path="";
                        outputmsg="Request ID: ";
                        out.writeObject(outputmsg);
                        inputmsg=(String) in.readObject(); //id
                        inputmsg=username+"_"+inputmsg+": ";

                        outputmsg="Description:";
                        out.writeObject(outputmsg);
                        outputmsg=inputmsg+(String) in.readObject(); //des
                       
                        FileWriter fwc;
                        for(Map.Entry<String, Info> clients: clientList.entrySet()){
                            if(clients.getKey().equalsIgnoreCase(username)){
                                path=filepath+"/"+username+"/Message/readmsg.txt";
                                fwc = new FileWriter(path,true);
                                fwc.append(outputmsg+" (Sent)\n");
                                fwc.close();
                            }
                            else {
                                path=filepath+"/"+clients.getKey()+"/Message/unreadmsg.txt";
                                fwc = new FileWriter(path,true);
                                fwc.append(outputmsg+"\n");
                                fwc.close();
                            }
                        }
                        outputmsg="Request sent\n";
                        out.writeObject(outputmsg);
                     }

                    //unread msg
                    else if(inputmsg.contains("5")){
                        outputmsg="";
                        inputmsg="";
                        FileWriter fw = new FileWriter(filepath+"/"+username+"/Message/readmsg.txt",true);

                        file1=new File(filepath+"/"+username+"/Message/unreadmsg.txt");
                        Scanner scnFile=new Scanner(file1);
                        while (scnFile.hasNextLine()){
                            inputmsg=scnFile.nextLine();
                            fw.append(inputmsg+" (Received)\n");
                            outputmsg+=inputmsg+"\n";
                        }
                        scnFile.close();
                        fw.close();
                        
                        FileWriter fw1 = new FileWriter(filepath+"/"+username+"/Message/unreadmsg.txt");
                        fw1.write("");
                        fw1.close();
                        out.writeObject(outputmsg);
                    }

                    //upload
                    else if(inputmsg.contains("6")){
                        String fpath="";
                        String fname="";
                        String type="";
                        String[] token;
                        long fsize;
                        double chu;
                        int chunck;
                        Boolean req=false;
                        String reqUser="";
                        String reqID="";
                        Boolean timeout=false;

                        outputmsg="Type: 1.Public 2.Private 3.Requested";
                        out.writeObject(outputmsg);
                        inputmsg=(String) in.readObject(); //type
                        if(inputmsg.contains("1")){
                            inputmsg="Public";
                        } else if (inputmsg.contains("2")) {
                            inputmsg="Private";
                        } else if (inputmsg.contains("3")){
                            out.writeObject("Request ID: ");
                            inputmsg=(String)in.readObject();
                            token=inputmsg.split("_");
                            reqUser=token[0];
                            reqUser=reqUser.toUpperCase();
                            reqID=inputmsg;
                            inputmsg="Public";
                            req=true;
                        }

                        fpath=filepath+"/"+username+"/"+inputmsg;
                        type=inputmsg;

                        outputmsg="File Name: ";
                        out.writeObject(outputmsg);
                        inputmsg=(String) in.readObject(); //name
                        fname=inputmsg;
                        outputmsg="File Size: ";
                        out.writeObject(outputmsg);
                        inputmsg=(String) in.readObject(); //size
                        fsize=Long.parseLong(inputmsg,10);

                        //check buffer overflow
                        //System.out.println("BufferSize "+Server.getCurBufferSize());
                        if((Server.getCurBufferSize()+fsize) > MAX_BUFFER_SIZE){
                            out.writeObject("Can not upload now!\nTry again later");
                        }
                        else{
                            chu=Math.floor(Math.random()*(MAX_CHUNCK_SIZE-MIN_CHUNCK_SIZE));
                            chunck=(int)Math.floor(chu+MIN_CHUNCK_SIZE);

                            outputmsg="Chunck Size:"+chunck;
                            out.writeObject(outputmsg);  //chunck size

                            outputmsg="FileId:"+Server.fileID;
                            out.writeObject(outputmsg);  //id
                            outputmsg=fname+":"+username+":"+type;
                            fileList.put(Server.fileID,outputmsg);
                            Server.fileID++;

                            fpath=fpath+"/"+fname;
                            FileOutputStream fo=new FileOutputStream(fpath);
                            try{
                                byteArr=new byte[chunck];
                                long siz=0;
                                while(true){
                                    Object o;
                                    o=in.readObject();
                                    if(o instanceof String){
                                        inputmsg=(String)o;
                                        System.out.println(username+": "+inputmsg);
                                        if(inputmsg.contains("Transmission timeout")){
                                            timeout=true;
                                        }
                                        break;
                                    }                           
                                    else {
                                        numOfBytes= (int) o;
                                        o=in.readObject();
                                        byteArr=(byte[]) o;
                                        siz+=numOfBytes;
                                        Server.changeCurBufferSize(numOfBytes);
                                        outputmsg="Chuncksize "+numOfBytes+" received";
                                        System.out.println(outputmsg);
                                        fo.write(byteArr,0,numOfBytes);
                                        
                                        // System.out.println("BufferSize "+Server.getCurBufferSize());
                                        //Thread.sleep(2000); //upload denied, timeout, socket close err check
                                        Server.changeCurBufferSize((-1)*numOfBytes);
                                        out.writeObject("Server: "+outputmsg);
                                    }
                                }
                            
                                fo.close();
                                //if size doesn't match
                                // siz=0;
                                if(fsize!=siz || timeout){
                                    file1=new File(fpath);
                                    file1.delete();
                                    System.out.println(fname+" deleted");
                                    out.writeObject("Server: Error!\n"+fname+" deleted");
                                }
                                else{
                                    System.out.println("File received");
                                    if(req){
                                        String path=filepath+"/"+reqUser+"/Message/unreadmsg.txt";
                                        FileWriter fwr = new FileWriter(path,true);
                                        fwr.append("Server: For request id "+reqID+", file '"+fname+"' is uploaded by '"+username+"'\n");
                                        fwr.close();
                                    }
                                    out.writeObject("Server: "+fname+" successfully uploaded");
                                }
                            } catch(Exception e){
                                System.out.println("Exception: "+e); 
                                fo.close();                         
                                file1=new File(fpath);
                                if(file1.exists()){
                                    file1.delete();
                                    System.out.println(fname+" deleted");  
                                }                          
                            }
                        }
                        // System.out.println(fileList);
                    }

                    //read msg
                    else if(inputmsg.contains("7")){
                        outputmsg="";                       
                        file1=new File(filepath+"/"+username+"/Message/readmsg.txt");
                        Scanner scnfile=new Scanner(file1);
                        while (scnfile.hasNextLine()){
                            inputmsg=scnfile.nextLine();
                            outputmsg+=inputmsg+"\n";
                        }
                        scnfile.close();
                        out.writeObject(outputmsg);
                    }

                    else if(inputmsg.contains("0")){
                        //System.out.println("User "+username+" logged out");
                        break;
                    }

                }
            }

        } catch (IOException e) {
            System.out.println("Exception:"+e);
        } catch (ClassNotFoundException e) {
            System.out.println("Exception:"+e);
        } catch (InterruptedException e) {
            System.out.println("Exception:"+e);
        } finally {
            try {
                if(flag && clientList.containsKey(username)){
                    clientInfo.online=false;
                }
                System.out.println(username+" logged out");
                socket.close();
            } catch (IOException e) {
                System.out.println(e);
            }
        }
    }
}
