import com.sun.tools.javac.util.ArrayUtils;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;

public class RemoteClient {

    public static void main(String[] args) throws Exception{
        /*
        String serverAddress = JOptionPane.showInputDialog(
                "Enter IP Address of a machine that is\n" +
                        "running the date service on port 80:");
                        */

        boolean isContinuous = true;
        boolean isRelay = false;

        while(isContinuous) {
            isRelay = !isRelay;

            String serverAddress = "192.168.1.101";
            Socket s = new Socket(serverAddress, 80);
            BufferedReader input = new BufferedReader(new InputStreamReader(s.getInputStream()));
            BufferedWriter output = new BufferedWriter(new OutputStreamWriter(s.getOutputStream()));

            // Read status
            String received = input.readLine();
            String[] receivedArray = received.split(",");
            System.out.println(
                    String.format("Voltage: %s, Current: %s, Relay: %s, Ratio: %s, Motor: %s",
                            receivedArray[0], receivedArray[1], receivedArray[2], receivedArray[3], receivedArray[4])
            );

            // Send response
            String response = "5," + (isRelay ? "1" : "0");
            output.write(response);
            output.flush();

            s.close();
        }
    }
}
