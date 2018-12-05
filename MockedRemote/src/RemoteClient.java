import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;

public class RemoteClient {

    public static void main(String[] args) throws Exception
    {
        /*
        String serverAddress = JOptionPane.showInputDialog(
                "Enter IP Address of a machine that is\n" +
                        "running the date service on port 80:");
                        */

//        mainbody("5,1");

        while(true)
        {
            mainbody("0,0");
        }

    }

    public static void mainbody(String response) throws Exception
    {
        String serverAddress = "192.168.1.101";
        Socket s = new Socket(serverAddress, 80);
        BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream()));
        BufferedWriter output = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));

        // Read status
        String received = input.readLine();
        String[] receivedArray = received.split(",");

        double voltage = ((double)Integer.parseInt(receivedArray[0])) / 1000.0d;
        double current = ((double)Integer.parseInt(receivedArray[1])) / 1000.0d;
        double temperature =  ((double)Integer.parseInt(receivedArray[5])) / 100.0d;

        System.out.println(
                String.format("Voltage: %5.3fV, Current: %5.3fA, Relay: %s, Ratio: %s, Motor: %s, Temp.: %4.2fC",
                        voltage, current, receivedArray[2], receivedArray[3], receivedArray[4], temperature)
        );

        // Send response
        output.write(response);
        output.flush();

        s.close();
    }
}
