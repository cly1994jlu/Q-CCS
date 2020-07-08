# Q-CCS
This contains the simulation code for the paper L. Chu, J. Zheng, and J. Xiao, “A QoS-guaranteed component carrier selection algorithm for LTE-U and WiFi coexistence networks”, in Proc. of IEEE Global Communication Conference (Globecom), Waikoloa, HI, USA, Dec. 2019. The fulltext of this paper can be found in https://ieeexplore.ieee.org/abstract/document/9013911.

The simulation experiments are implemented in C++, compiled in Microsoft Visual Studio 2013. Data are collected and then processed in MATLAB to generate figures in the paper. For more details of the experiments and results, you can consult my master thesis, which can be found in cnki.net.

These simulations are based on event-driven simulations, similar to a bank queueing system, to model the arrival and departure process of users.

If you are interested in the simulation codes, please cite the paper as

@INPROCEEDINGS{Chu_GC19,
  author={L. {Chu} and J. {Zheng} and J. {Xiao}},
  booktitle={2019 IEEE Global Communications Conference (GLOBECOM)}, 
  title={A QoS-Guaranteed Component Carrier Selection Algorithm for LTE-U and WiFi Coexistence Networks}, 
  month={Dec.},
  year={2019},
  address={Waikoloa, HI, USA},
  volume={},
  number={},
  pages={1-6},}
