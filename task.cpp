#include "api.hpp"
#include <string>
#include <set>
#include <queue>
#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;

void del_unreach(DFA &d) {
  set<string> used, states = d.get_states();
  queue<string> q;
  q.push(d.get_initial_state());
  used.insert(d.get_initial_state());
  while (!q.empty()) {
    string state = q.front();
    q.pop();
    for(auto symbol : d.get_alphabet()) {
      if (d.has_trans(state, symbol) && used.find(d.get_trans(state, symbol)) == used.end()) {
        q.push(d.get_trans(state, symbol));
        used.insert(d.get_trans(state, symbol));
      }
    }
  }
  set<string> diff;
  set_difference(states.begin(), states.end(), used.begin(), used.end(), inserter(diff, diff.begin()));
  for(auto state : diff) {
    d.delete_state(state);
  }
}

void complement(DFA &d) {
  set<string> states = d.get_states();
  Alphabet alphabet = d.get_alphabet();
  d.create_state("deadlock", false);
  states.insert("deadlock");
  bool mark = true;
  for(auto state : states) {
    for(auto symbol : alphabet) {
      if (!d.has_trans(state, symbol)) {
        d.set_trans(state, symbol, "deadlock");
        mark = false;
      }
    }
  }
  if (mark) {
    d.delete_state("deadlock");
  }
}

pair<set<string>,set<string>> split(pair<set<string>, bool> &r, pair<set<string>, char> &curr, DFA & d) {
  pair<set<string>, set<string>> r1r2;
  for(auto state : r.first) {
    if (curr.first.find(d.get_trans(state, curr.second)) != curr.first.end()) {
      r1r2.first.insert(state);
    } else {
      r1r2.second.insert(state);
    }
  }
  return r1r2;
}

DFA dfa_minim(DFA &d) {
  del_unreach(d);
  complement(d);
  Alphabet alphabet = d.get_alphabet();
  set<string> states = d.get_states();
  queue<pair<set<string>, char>> q;
  set<string> start, end;
  for(auto state : states) {
    if (d.is_final(state)) {
      end.insert(state);
    } else {
      start.insert(state);
    }
  }
  for(auto symbol : alphabet) {
    q.push(make_pair(start, symbol));
    q.push(make_pair(end, symbol));
  }
  set<pair<set<string>, bool>> p;
  p.insert(make_pair(start, false));
  p.insert(make_pair(end, true));
  while (!q.empty()) {
    pair<set<string>, char> curr = q.front();
    q.pop();
    for(auto r : p) {
      pair<set<string>, set<string>> r1r2 = split(r, curr, d);
      if (!r1r2.first.empty() && !r1r2.second.empty()) {
        p.erase(r);
        if (r.second) {
          p.insert(make_pair(r1r2.first, true));
          p.insert(make_pair(r1r2.second, true));
        } else {
          p.insert(make_pair(r1r2.first, false));
          p.insert(make_pair(r1r2.second, false));
        }
        for(auto symbol : alphabet) {
          q.push(make_pair(r1r2.first, symbol));
          q.push(make_pair(r1r2.second, symbol));
        }
      }
    }
  }
  // for(auto a : p) {
  //   for(auto b : a.first) {
  //     cout << b;
  //   }
  //   cout << endl;
  // }
  DFA res = DFA(alphabet);
  int i = 0;
  for(auto state : p) {
    res.create_state(to_string(i), state.second);
    i++;
  }
  for(auto symbol : alphabet) {
    for(int j=0; j<i; j++) {
      int k = 0;
      set<string> left;
      for(auto state : p) {
        if (k == j) {
          left = state.first;
          break;
        }
        k++;
      }
      string stat = d.get_trans(*left.begin(), symbol);
      k = 0;
      for(auto state : p) {
        if (state.first.find(stat) != state.first.end()) {
          break;
        }
        k++;
      }
      res.set_trans(to_string(j), symbol, to_string(k));
    }
  }
  string initial = d.get_initial_state();
  int j = 0;
  for(auto state : p) {
    if (state.first.find(initial) != state.first.end()) {
      res.set_initial(to_string(j));
      initial = to_string(j);
      break;
    }
    j++;
  }
  set<string> curr_states;
  for(j=0; j<i; j++) {
    curr_states.insert(to_string(j));
  }
  bool mark = true;
  while(mark) {
    mark = false;
    // set<string> new_states;
    // for(auto j : curr_states) {
    //   new_states.insert(j);
    // }
    // new_states.erase(initial);
    // for(auto j : curr_states) {
    //   for(auto symbol : alphabet) {
    //     if (res.has_trans(j, symbol)) {
    //       new_states.erase(res.get_trans(j, symbol));
    //     }
    //   }
    // }
    // for(auto state : new_states) {
    //   res.delete_state(state);
    //   curr_states.erase(state);
    //   mark = true;
    // }
    for(j=0; j<i; j++) {
      if (!res.is_final(to_string(j)) && res.has_state(to_string(j))) {
        bool mark_self = true;
        for(auto symbol : alphabet) {
          if(res.has_trans(to_string(j), symbol) && res.get_trans(to_string(j), symbol) != to_string(j)) {
            mark_self = false;
          }
        }
        if (mark_self) {
          res.delete_state(to_string(j));
          curr_states.erase(to_string(j));
          // cout << j << endl;
          mark = true;
        }
      }
    }
  }
  return res;
}
