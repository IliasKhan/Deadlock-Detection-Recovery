#include<bits/stdc++.h>


using namespace std;

void print(vector<bool> v){
  cout<<endl;
  for(auto p : v){
    cout<<p<<" ";
  }
  cout<<endl;
}


//Function to detect & recover deadlock
void deadlockDetectionAndRecovery(vector<vector<int>> &wait_graph, int pid_probe, vector<int> all_processes, vector<bool> &terminated_processes);

//Function to detect deadlock
bool isDeadlockDetected(vector<vector<int>> &wait_graph, int pid_probe, vector<int> all_processes, vector<bool> &terminated_processes);
bool detectDeadlock(vector<vector<int>> &wait_graph, int pid_probe, int dest_process, vector<int> all_processes, vector<bool> &terminated_processes);

//Function to Recover deadlock
void recoverDeadlock(vector<vector<int>> &wait_graph, vector<int> all_processes, vector<bool> &terminated_processes);

//Function to display graph
void displayGraph(vector<vector<int>> mat);

//Number of machine in the distributed system
int number_of_site;
vector<int> priority_wise_processes;

// The main execution starts from here
int main() {

// For input/output files
  ifstream in("input.txt");
  streambuf *cinbuf = std::cin.rdbuf(); 
  cin.rdbuf(in.rdbuf()); 

  ofstream out("output.txt");
  streambuf *coutbuf = std::cout.rdbuf(); 
  cout.rdbuf(out.rdbuf()); 
  
// Taking Input & building wait graph from i/p files
  cin >> number_of_site;
  cout << "Number of sites/machines: "<<number_of_site<<"\n"<< endl;

  cout << "Priority wise Process sites\n";
  for(int idx=0; idx < number_of_site; idx++){
    int process_number;
    cin >> process_number;
    cout<<process_number<<" ";
    priority_wise_processes.push_back(process_number);
  }

  cout<<"\n\nProcess Info(ordered by local dependancy)\n";
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

  vector<bool> terminated_processes(number_of_site, false);
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
    cout << "First process of Site "<< pid_probe <<" pid_prob initiated the detection process"<< endl;
    pid_probe = pid_probe - 1;

    //Deadlock Detection And Recovery
    deadlockDetectionAndRecovery(wait_graph, pid_probe, all_processes, terminated_processes);
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

void deadlockDetectionAndRecovery(vector<vector<int>> &wait_graph, int pid_probe, vector<int> all_processes, vector<bool> &terminated_processes){
  if(isDeadlockDetected(wait_graph, pid_probe, all_processes, terminated_processes)){
    cout<<"Deadlock Detected";
    recoverDeadlock(wait_graph, all_processes, terminated_processes);
  }
  else
    cout<<"No deadlock has been detected";
}

bool isDeadlockDetected(vector<vector<int>> &wait_graph, int pid_probe, vector<int> all_processes, vector<bool> &terminated_processes){
  int first_pid = pid_probe;
  for (int col = 0; col < number_of_site; col++)
  {
    if(terminated_processes[col])
      continue;
    if (wait_graph.at(pid_probe).at(col) == 1)
    {
      cout << " S" << (pid_probe + 1) << " --> S" << (col + 1) << "     (" <<"p"<<(first_pid + 1)<<"1"<< ", " <<"p"<<(pid_probe + 1)<<all_processes[pid_probe] << ", " << "p"<<(col + 1)<<1<< ")" << endl;
      if(detectDeadlock(wait_graph, pid_probe, col, all_processes, terminated_processes))
      {
        return true;
      }
    }
  }
  return false;
}

bool detectDeadlock(vector<vector<int>> &wait_graph, int pid_probe, int dest_process, vector<int> all_processes, vector<bool> &terminated_processes)
{
  int end = number_of_site;
  for (int col = 0; col < end; col++)
  {
    if(terminated_processes[col])
      continue;
    if (wait_graph[dest_process][col] == 1)
    {
      if (pid_probe == col)
      {
        cout << " S" << (dest_process + 1) << " --> S" << (col + 1) <<"     (" <<"p"<< (pid_probe + 1) <<"1"<< ", " << "p"<<(dest_process + 1)<<all_processes[dest_process] << ", " << "p" << (col + 1) <<1<< ")" << " --------> DEADLOCK DETECTED HERE" << endl;
        return true;
        break;
      }
      cout << " S" << (dest_process + 1) << " --> S" << (col + 1) << "     (" <<"p"<< (pid_probe + 1)<<"1" << ", " <<"p"<< (dest_process + 1) <<all_processes[dest_process] << ", "<<"p"<< (col + 1) <<1<< ")" << endl;
      if(detectDeadlock(wait_graph, pid_probe, col, all_processes, terminated_processes))
        return true;
    }
  }
  return false;
}
int getProcessByPriority(vector<bool> &terminated_processes){
  for(int i=0;i<number_of_site;i++){
    if(terminated_processes[priority_wise_processes[i]-1]){
      continue;
    }
    return priority_wise_processes[i]-1;
  }
  return -1;
}

int get_probe(vector<bool> &terminated_processes){
  for(int site_number=0;site_number<number_of_site;site_number++){
    if(terminated_processes[site_number])
      continue;
    return site_number;
  }
  return -1;
}

void recoverDeadlock(vector<vector<int>> &wait_graph, vector<int> all_processes, vector<bool> &terminated_processes){
  int suicide_process = getProcessByPriority(terminated_processes);
  terminated_processes[suicide_process] = 1;//killed suicide_process process
  int pid_probe = get_probe(terminated_processes);
  if(pid_probe == -1)
  {
    cout<<"All process has been deleted, We can restart the distributed system";
    return;
  }
  cout<<"\n\nProcess "<<suicide_process+1<<" has been killed\n";
  // print(terminated_processes);
  if(isDeadlockDetected(wait_graph, pid_probe, all_processes, terminated_processes)){
    recoverDeadlock(wait_graph, all_processes, terminated_processes);
  }
  else{
    cout << "Deadlock has been recovered";
  }
}





