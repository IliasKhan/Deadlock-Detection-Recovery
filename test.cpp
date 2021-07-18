#include<bits/stdc++.h>


using namespace std;

//Function to detect deadlock
bool detectDeadlock(vector<vector<int>> &wait_graph, int pid_probe, int dest_process, vector<int> all_processes);

//Function to display graph
void displayGraph(vector<vector<int>> mat);

//Number of machine in the distributed system
int number_of_site;


bool is_deadlock_detected = false;

// The main execution starts from here
int main() {

  ifstream in("input.txt");
  streambuf *cinbuf = std::cin.rdbuf(); //save old buf
  cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!

  ofstream out("output.txt");
  streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  cout.rdbuf(out.rdbuf()); 
  

  cin >> number_of_site;
  cout << "Number of sites/machines: "<<number_of_site<<"\n"<< endl;
  // cout<<": "<<number_of_site<<"\n";

  cout<<"Process Info(ordered by local dependancy)\n";
  vector<int> all_processes;
  for(int site_no=0; site_no<number_of_site; site_no++){
    int process_count;
    cin>>process_count;
    all_processes.push_back(process_count);
    cout<<"site-"<<site_no+1<<": ";
    for(int pindex=0; pindex < process_count; pindex++){
      cout<<"p"<<site_no+1<<pindex+1<<" ";
    }
    cout<<endl;
  }
  cout<<"\n";

  


  if (number_of_site > 1) 
  {
    vector<vector<int>> wait_graph(number_of_site);    

    //Input the wait graph values
    for (int from = 0; from < number_of_site; from++)
    {
      for (int to = 0; to < number_of_site; to++)
      {
        int temp;
        cin >> temp;
        wait_graph.at(from).push_back(temp);
      }
    }

    cout << endl;
    cout << "Wait-For-Graph for Sites"<<"\n\n";

    //display wait for graph
    displayGraph(wait_graph);
    cout << endl;

    //Enter the proccess pid_probeiating the probe
    
    int pid_probe, first_pid;
    cin >> pid_probe;
    cout << "First process of Site "<< pid_probe <<" pid_probeiated the detection process"<< endl;
    pid_probe = pid_probe - 1;
    first_pid = pid_probe;

    //Detecting deadlock
    for (int col = 0; col < number_of_site; col++)
    {
      if (wait_graph.at(pid_probe).at(col) == 1)
      {
        cout << " S" << (pid_probe + 1) << " --> S" << (col + 1) << "     (" <<"p"<<(first_pid + 1)<<"1"<< ", " <<"p"<<(pid_probe + 1)<<all_processes[pid_probe] << ", " << "p"<<(col + 1)<<1<< ")" << endl;
        if(detectDeadlock(wait_graph, pid_probe, col, all_processes))
        {
          cout<<"Deadlock Detected";
          return 0;
        }
      }
    }
    cout<<"No deadlock has been detected";
  }
  else {
    cout << "Deadlock detection not possbile. No proccess running in the system" << endl;
  }

  return 0;
}

void displayGraph(vector<vector<int>> wait_graph)
{
  int n = wait_graph.at(0).size();
  int m = wait_graph.size();
  
  //Top Column
  cout << "\t";
  for (int j = 0; j < m; j++) {
    cout << "S" << (j + 1) << "\t";
  }
  cout << endl; 
  //Side column and values
  for (int i = 0; i<m; i++)
  {
    cout << "S" << (i + 1) << "\t";
    for (int j = 0; j<n; j++)
    {
      cout << wait_graph.at(i).at(j) << "\t";
    }
    cout << "\n";
  }
}

bool detectDeadlock(vector<vector<int>> &wait_graph, int pid_probe, int dest_process, vector<int> all_processes)
{
  int end = number_of_site;
  for (int col = 0; col < end; col++)
  {
    if (wait_graph[dest_process][col] == 1)
    {
      if (pid_probe == col)
      {
        cout << " S" << (dest_process + 1) << " --> S" << (col + 1) <<"     (" <<"p"<< (pid_probe + 1) <<"1"<< ", " << "p"<<(dest_process + 1)<<all_processes[dest_process] << ", " << "p" << (col + 1) <<1<< ")" << " --------> DEADLOCK DETECTED HERE" << endl;
        is_deadlock_detected = 1;
        return true;
        break;
      }
      cout << " S" << (dest_process + 1) << " --> S" << (col + 1) << "     (" <<"p"<< (pid_probe + 1)<<"1" << ", " <<"p"<< (dest_process + 1) <<all_processes[dest_process] << ", "<<"p"<< (col + 1) <<1<< ")" << endl;
      if(detectDeadlock(wait_graph, pid_probe, col, all_processes))
        return true;
    }
  }
  return false;
}